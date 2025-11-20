#version 400

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
uniform mat4 mvpMatrix;
out vec3 vPosition;
out vec3 vNormal;
out vec4 vColor;

void main()
{
  gl_Position = mvpMatrix * position;
  vPosition = vec3(mvMatrix * position);
  vNormal = normalize(normalMatrix * normal);
  vColor = color;
}