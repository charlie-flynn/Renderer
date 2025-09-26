#version 430 core

in vec2 vertUV;
in vec3 vertNormal;

layout (location = 3) uniform sampler2D albedo;
layout (location = 4) uniform vec3 ambientLight;
layout (location = 5) uniform vec3 lightDirection;

out vec4 outColor;

void main()
{
	float lambertDirection = max(0, dot(vertNormal, -lightDirection));
	vec3 diffuse = vec3(lambertDirection);
	vec4 baseColor = texture(albedo, vertUV);

	outColor.xyz = baseColor.xyz * (ambientLight + diffuse);
	outColor.a = baseColor.a;
}