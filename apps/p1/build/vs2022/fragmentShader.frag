#version 400

#define PI 3.1415926535897

struct LightProps
{
	int type; // DIRECTIONAL/POINT/SPOT
	vec4 color; // color
	vec3 position; // VRC position
	vec3 direction; // VRC direction
	int falloff; // CONSTANT/LINEAR/QUADRATIC
	float range; // range (== 0 INFINITE)
	float angle; // spot angle
};

struct MaterialProps
{
	vec4 Oa; // ambient color
	vec4 Od; // diffuse color
	vec4 Os; // specular spot color
	float shine; // specular shininess exponent
	float rugosity; // rugosity factor
	float metalFactor; // metal factor
};



in vec3 v_position;
in vec3 v_normal;
in vec4 v_color;

uniform int mode;
uniform vec4 ambientLight;
uniform int lightCount;
uniform LightProps lights[8];
uniform MaterialProps material;

layout(location = 0) out vec4 fragmentColor;



bool lightVector(int i, vec3 P, out vec3 L, out float d)
{
	int type = lights[i].type;

	// DIRECTIONAL
	if (type == 0)
	{
		L = -lights[i].direction;
		return true;
	}
	L = lights[i].position - P;
	d = length(L);

	float range = lights[i].range;

	if (d == 0 || (range > 0 && d > range))
		return false;
	L /= d;
	// POINT
	if (type == 1)
		return true;

	// SPOT
	float DL = dot(lights[i].direction, L);

	return DL < 0 && lights[i].angle > radians(acos(DL));
}

vec4 lightColor(int i, float d)
{
	int falloff = lights[i].falloff;

	// directional light or constant falloff
	if (lights[i].type == 0 || falloff == 0)
		return lights[i].color;

	float range = lights[i].range;
	float f;

	if (range == 0) // infinite range
	{
		f = 1 / d;
		if (falloff == 2) // quadratic falloff
			f *= f;
	}
	else
	{
		f = d / range;
		f = falloff == 2 ? 1 + f * (f - 2) : 1 - f;
	}
	return lights[i].color * f;
}

vec4 phong(vec3 P, vec3 N, MaterialProps m)
{
	vec4 color;

	color = ambientLight * m.Oa;

	vec3 V = normalize(P);

	if (dot(N, V) > 0)
		//return backFaceColor;
		N *= -1;

	vec3 R = reflect(V, N);

	for (int i = 0; i < lightCount; i++)
	{
		vec3 L; float d;

		if (lightVector(i, P, L, d))
		{
			vec4 I = lightColor(i, d);
			color += I * m.Od * max(dot(N, L), 0);
			color += I * m.Os * pow(max(dot(R, L), 0), m.shine);
		}
	}
	return min(color, vec4(1));
}

vec4 interpolateColor(vec4 min, vec4 max, float t)
{
	return min + (max - min)*t;
}

vec4 BRDF(vec3 P, vec3 N, MaterialProps m)
{
	vec4 color;
	color = m.Oa;
	
	
	vec3 V = normalize(P);

	if(dot(N,V) > 0)
		N *= -1;

	vec4 diffuseBRDF = interpolateColor(m.Od, vec4(0), m.metalFactor) / PI;
	for(int i = 0; i < lightCount; i++)
	{
		vec3 L; float d;
		
		if(lightVector(i, P, L, d))
		{
			vec4 lightColor = lightColor(i, d);
			vec3 Hl = normalize(L - V);
			vec4 interpolatedSpecular = interpolateColor(vec4(0.04)*m.Os, m.Os, m.metalFactor);
			vec4 fresnel = interpolatedSpecular + (vec4(1) - interpolatedSpecular) * pow(1 - max(dot(L, Hl), 0.01), 5);

			float nDotL = max(dot(N, L), 0.01);
			float nDotV = max(dot(N, -V), 0.01);
			float k = (pow(m.rugosity + 1, 2)) / 8;

			float g1 = nDotL / ( (nDotL * (1 - k)) + k);
			float g2 = nDotV / ( (nDotV * (1 - k)) + k);
			
			float alpha = pow(m.rugosity, 2);
			float microfacetNDF = alpha*alpha / ( PI * pow( pow(max(dot(N, Hl), 0.01), 2) * (alpha*alpha - 1) + 1, 2));

			vec4 specularBRDF = fresnel * (g1 * g2 * microfacetNDF) / (4 * nDotL * nDotV );
			
			color += lightColor * (diffuseBRDF + specularBRDF) * nDotL;
		}
	}

	return min(PI*color, vec4(1));
}

void main()
{
	if(mode == 1)
		fragmentColor = BRDF(v_position, v_normal, material);
	else
		fragmentColor = phong(v_position, v_normal, material);
}