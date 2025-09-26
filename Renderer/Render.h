#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace aie
{
	struct Vertex
	{
		glm::vec4 Pos;
		glm::vec2 UVs;
	};

	struct Geometry
	{
		GLuint Vao = 0, Vbo = 0, Ibo = 0; // Buffer names
		GLuint Size = 0;				  // Index Count
	};

	Geometry MakeGeometry(
		const Vertex* const Verts,
		GLsizei VertCount,
		const GLuint* const indices,
		GLsizei IndexCount);

	void FreeGeometry(Geometry& Geo);

	struct Texture
	{
		GLuint Handle;						  // opengl texture name
		unsigned int Width, Height, Channels; // would be smart to have this data
	};

	/// <summary>
	/// Creates a texture in OpenGL
	/// </summary>
	/// <param name="width">Texture width (in pixels)</param>
	/// <param name="height">Texture height (in pixels)</param>
	/// <param name="channels">Number of channels (1 - R, 2 - RG, 3 - RGB, 4 - RGBA)</param>
	/// <param name="pixels">Pointer to pixel buffer</param>
	/// <returns>The texture.</returns>
	Texture MakeTexture(unsigned int width, unsigned int height, unsigned int channels, const unsigned char* pixels);
	Texture LoadTexture(const char* imagePath);
	void FreeTexture(Texture& tex);

	struct Shader
	{
		GLuint Program;
	};

	Shader ReadShaderFromFiles(const char* vertShaderPath, const char* fragShaderPath);
	Shader MakeShader(const char* vertSource, const char* fragSource);
	void FreeShader(Shader& shad);

	void SetUniform(const Shader& shad, GLuint location, const glm::mat4 value);
	void SetUniform(const Shader& shad, GLuint location, const float value);
	void SetUniform(const Shader& shad, GLuint location, const Texture& texture, int textureSlot);

	void Draw(const Shader& shad, const Geometry& geo);
}