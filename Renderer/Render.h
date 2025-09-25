#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace aie
{
	struct Vertex
	{
		glm::vec4 Pos;
		glm::vec2 UV;
	};

	struct Geometry
	{
		GLuint Vao = 0, Vbo = 0, Ibo = 0; // Buffer names
		GLuint Size = 0;				  // Index Count

		glm::vec2 UVs[];				  // UVs i dont know what that stands for tbh
	};

	Geometry MakeGeometry(
		const Vertex* const Verts,
		GLsizei VertCount,
		const GLuint* const indices,
		GLsizei IndexCount,
		const glm::vec2* const uvs,
		GLsizei UVCount);

	void FreeGeometry(Geometry& Geo);

	struct Texture
	{
		GLuint Handle;
		unsigned int Width, Height, Channels;
	};

	Texture MakeTexture(unsigned int width, unsigned int height, unsigned int channels, const unsigned char* pixels);
	Texture LoadTexture(const char* imagePath);
	void FreeTexture(Texture& tex);

	struct Shader
	{
		GLuint Program;
	};

	const char* ReadShader(const char* Path);
	Shader MakeShader(const char* vertSource, const char* fragSource);
	void FreeShader(Shader& shad);

	void SetUniform(const Shader& shad, GLuint location, const glm::mat4 value);
	void SetUniform(const Shader& shad, GLuint location, const float value);

	void Draw(const Shader& shad, const Geometry& geo);
}