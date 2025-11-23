#version 400

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
};



in vec3 v_position;
in vec3 v_normal;
in vec4 v_color;

uniform int projectionType; // PERSPECTIVE/PARALLEL
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

vec4 phong(vec3 P, vec3 N)
{
	MaterialProps m;
	vec4 color;

	color = ambientLight * m.Oa;

	vec3 V = projectionType == 0 ?
		// PERSPECTIVE
		normalize(P) :
		// PARALLEL
		vec3(0, 0, -1);

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


void main()
{
	fragmentColor = phong(v_position, v_normal);
}