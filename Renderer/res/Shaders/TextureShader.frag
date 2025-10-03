#version 430 core

in vec2 vertUV;
in vec3 vertNormal;

layout (location = 3) uniform sampler2D albedo;
layout (location = 4) uniform vec3 ambientLight;

layout (location = 5) uniform vec3 lightDirection;
layout (location = 6) uniform vec3 secondLightDirection;

layout (location = 7) uniform vec3 light1Color;
layout (location = 8) uniform vec3 light2Color;

out vec4 outColor;

void main()
{
	float lambertDirection = max(0, dot(vertNormal, -lightDirection));
	float secondLambertDirection = max(0, dot(vertNormal, -secondLightDirection));

	vec3 diffuse = vec3(lambertDirection);
	vec3 secondDiffuse = vec3(secondLambertDirection);

	vec4 baseColor = texture(albedo, vertUV);
	
	outColor.xyz = baseColor.xyz * (ambientLight);
	outColor.xyz = outColor.xyz * (lambertDirection + light1Color);
	outColor.xyz = outColor.xyz * (secondLambertDirection + light2Color);
	outColor.a = baseColor.a;
}