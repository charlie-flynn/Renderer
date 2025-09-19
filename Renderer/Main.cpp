#include <iostream>
#include <glm/ext.hpp>


#include "Context.h"
#include "Render.h"

using aie::Context;
using aie::Shader;
using aie::Vertex;
using aie::Geometry;


int main()
{
	Context Window;
	Window.Init(640, 480, "my awesome window <3");

	Vertex triVerts[] =
	{
		{{ -0.5f, -0.5f, 0, 1}},
		{{0.5f, -0.5f, 0, 1}},
		{{0, 0.5f, 0, 1}}
	};

	unsigned int triIndices[] = { 0, 1, 2 };

	Geometry basicTriangleGeo = aie::MakeGeometry(triVerts, 3, triIndices, 3);

	glm::mat4 triangle_model = glm::identity<glm::mat4>();
	glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 1, 0));

	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 1, 20),
		glm::vec3(0, 5, 0),
		glm::vec3(0, 1, 0));

	glm::mat4 cam_proj = glm::perspective(glm::radians(45.0f),
		640.0f / 480.0f,
		0.1f,
		1000.0f);

	const char* basicVert = aie::ReadShader("Shaders/VertexShader.txt");
	const char* basicFrag = aie::ReadShader("Shaders/BasicFragmentShader.txt");
	Shader basicShad = aie::MakeShader(basicVert, basicFrag);

	while (!Window.ShouldClose())
	{
		Window.Tick();
		Window.Clear();


		aie::SetUniform(basicShad, 0, cam_proj);
		aie::SetUniform(basicShad, 1, cam_view);
		aie::SetUniform(basicShad, 2, triangle_model);

		aie::Draw(basicShad, basicTriangleGeo);

	}

	aie::FreeShader(basicShad);
	aie::FreeGeometry(basicTriangleGeo);

	Window.Terminate();

	return 0;
}