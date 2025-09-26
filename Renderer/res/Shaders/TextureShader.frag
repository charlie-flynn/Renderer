#version 430 core

in vec2 vertUV;

layout (location = 3) uniform sampler2D albedo;

out vec4 outColor;

void main()
{
	outColor = texture(albedo, vertUV);
}