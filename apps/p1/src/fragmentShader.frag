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

struct LineProps
{
  float width;
  vec4 color;
};

subroutine vec4 mixColorType(vec4 color);
subroutine void matPropsType(out MaterialProps m);

in vec3 gPosition;
in vec3 gNormal;
in vec4 gColor;
noperspective in vec3 gEdgeDistance;
//uniform vec4 backFaceColor = vec4(1, 0, 1, 1);
uniform int projectionType; // PERSPECTIVE/PARALLEL
uniform vec4 ambientLight;
uniform int lightCount;
uniform LightProps lights[8];
uniform MaterialProps material;
uniform LineProps line;
subroutine uniform mixColorType mixColor;
subroutine uniform matPropsType matProps;
layout(location = 0) out vec4 fragmentColor;

subroutine(matPropsType)
void modelMaterial(out MaterialProps m)
{
  m = material;
}

subroutine(matPropsType)
void colorMapMaterial(out MaterialProps m)
{
  const float Oa = 0.4;
  const float Od = 0.6;

  m = MaterialProps(gColor * Oa, gColor * Od, vec4(1), material.shine);
}

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
  if (lights[i].type == 0  || falloff == 0)
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

  matProps(m);
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

subroutine(mixColorType)
vec4 noMix(vec4 color)
{
  return color;
}

subroutine(mixColorType)
vec4 lineColorMix(vec4 color)
{
  float d = min(min(gEdgeDistance.x, gEdgeDistance.y), gEdgeDistance.z);
  float mixVal;

  if (d < line.width - 1)
    mixVal = 1;
  else if (d > line.width + 1)
    mixVal = 0;
  else
  {
    float x = d - (line.width - 1);
    mixVal = exp2(-2 * (x * x));
  }
  return mix(color, line.color, mixVal);
}

void main()
{
  fragmentColor = mixColor(phong(gPosition, normalize(gNormal)));
}