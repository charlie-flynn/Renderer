#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace aie
{
	struct Vertex
	{
		glm::vec4 Pos;
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



	struct Shader
	{
		GLuint Program;
	};

	const char* ReadShader(const char* Path);
	Shader MakeShader(const char* vertSource, const char* fragSource);
	void FreeShader(Shader& shad);

	void SetUniform(const Shader& shad, GLuint location, const glm::mat4 value);

	void Draw(const Shader& shad, const Geometry& geo);
}