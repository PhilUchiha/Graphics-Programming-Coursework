#version 330 core

uniform vec3 lightColour;
uniform vec3 modelColour;
uniform vec3 lightPos;
uniform vec3 camPos;

out vec4 fragColour;

in vec3 normal;
in vec3 fragPos;

void main()
{
//ambient
	float ambStrength = 0.1f;
	vec3 A = (ambStrength * lightColour);

//diffuse
	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDirection), 0.0f);
	vec3 D = diff * lightColour;

//specular
	float specStrength = 0.5f;
	vec3 cameraDirection = normalize(camPos - fragPos);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(cameraDirection, reflectDirection), 0.0), 32);
	vec3 S = specStrength * spec * lightColour;

//ADS (Combining)
	vec3 ADSlight = (A + D + S) * modelColour;
	fragColour = vec4(ADSlight, 1.0);
}