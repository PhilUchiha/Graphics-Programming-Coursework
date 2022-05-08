#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec3 vertexNormal;

out vec3 normal;
out vec3 fragmentPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
	fragmentPosition = vec3(model * vec4(vertexPosition, 1.0f));
	normal = mat3(transpose(inverse(model))) *vertexNormal;
}