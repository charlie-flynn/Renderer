#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Context.h"
#include "Render.h"
#include "Util.h"

using aie::Context;
using aie::Shader;
using aie::Vertex;
using aie::Geometry;


int main()
{
	Context Window;
	Window.Init(640, 480, "my awesome window <3");

	Geometry soulSpearGeometry = aie::LoadGeometry("res/Models/soulspear.obj");

	// model matrix: puts object in world space
	glm::mat4 soulSpearModel = glm::identity<glm::mat4>();

	// view matrix: puts things relative to where the camera is
	glm::mat4 cameraView = glm::lookAt(
		glm::vec3(0, -5, 5), // eye
		glm::vec3(0, 2, 0),  // look at position
		glm::vec3(0, 1, 0)); // which way is up

	glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 1, 0));

	glm::mat4 cam_proj = glm::perspective(
		glm::radians(45.0f), // FOB
		640.0f / 480.0f,     // Aspect Ratio
		0.1f,                // Near Plane
		1000.0f);            // Far Plane

	Shader basicShad = aie::ReadShaderFromFiles("res/Shaders/CameraVertexShader.txt", "res/Shaders/TextureShader.frag");

	aie::Texture awesome = aie::LoadTexture("res/Textures/soulspear_specular.tga");
	
	glm::vec3 ambientLight = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 sunlightDirection(0, 0, 1);

	while (!Window.ShouldClose())
	{
		Window.Tick();
		Window.Clear();

		// setup the all important uniforms
		aie::SetUniform(basicShad, 0, cam_proj);
		aie::SetUniform(basicShad, 1, cameraView);
		aie::SetUniform(basicShad, 2, soulSpearModel);

		aie::SetUniform(basicShad, 3, awesome, 0);
		aie::SetUniform(basicShad, 4, ambientLight);
		aie::SetUniform(basicShad, 5, sunlightDirection);

		aie::Draw(basicShad, soulSpearGeometry);
	}
	
	aie::FreeShader(basicShad);
	aie::FreeGeometry(soulSpearGeometry);

	Window.Terminate();

	return 0;
}