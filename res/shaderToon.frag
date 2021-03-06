uniform vec3 lightDir;
in vec4 normal;

void main()
{
	float intensity;
	vec4 color;

	intensity = dot(lightDir, normal);

	if (intensity > 0.95)
		color = vec4(1.0,0.7,0.7,1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.9,0.9,0.8);
	else if (intensity > 0.25)
		color = vec4(0.4,0.6,0.2,0.2);
	else
		color = vec4(0.2,0.1,0.6,0.5);
	
	gl_FragColor = color;
}