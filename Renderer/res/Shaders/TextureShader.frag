#version 430 core

in vec2 vertUV;

layout (location = 3) uniform sampler2D albedo;
layout (location = 4) uniform vec3 ambientLight;

out vec4 outColor;

void main()
{
	vec4 baseColor = texture(albedo, vertUV);

	outColor.rgb = baseColor.rgb * ambientLight;
	outColor.a = baseColor.a;
}