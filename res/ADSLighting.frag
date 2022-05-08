#version 330 core

uniform vec3 lightColour;
uniform vec3 modelColour;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

out vec4 fragmentColour;

in vec3 normal;
in vec3 fragmentPosition;

void main()
{
//ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColour;
//diffuse
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightPosition - fragmentPosition);
	float diff = max(dot(norm, lightDirection), 0.0f);
	vec3 diffuse = diff * lightColour;
//specular
	float specularStrength = 0.5f;
	vec3 cameraDirection = normalize(cameraPosition - fragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(cameraDirection, reflectDirection), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColour;
//ADS (Combining)
	vec3 result = (ambient + diffuse + specular) * modelColour;
	fragmentColour = vec4(result, 1.0);
}