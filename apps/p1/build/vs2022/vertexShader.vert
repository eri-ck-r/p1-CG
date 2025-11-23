#version 400

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;

uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
uniform mat4 mvpMatrix;

out vec3 v_position;
out vec3 v_normal;
out vec4 v_color;

void main()
{
	gl_Position = mvpMatrix * position;
	v_position = vec3(mvMatrix * position);
	v_normal = normalize(normalMatrix * normal);
	v_color = color;
}