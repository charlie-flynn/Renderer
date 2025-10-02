#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Context.h"
#include "Render.h"
#include "Util.h"
#include "Transform.h"

using aie::Context;
using aie::Shader;
using aie::Vertex;
using aie::Geometry;


int main()
{
	Context Window;
	Window.Init(640, 480, "my awesome window <3");
	Window.CameraMovementSpeed = 1.0f;
	Window.CameraRotationSpeed = 1.0f;

	Geometry soulSpearGeometry = aie::LoadGeometry("res/Models/soulspear.obj");

	Transform soulSpearTransform = Transform();

	// model matrix: puts object in world space
	glm::mat4 soulSpearModel = soulSpearTransform.LocalMatrix();

	glm::vec3 eye = glm::vec3(0, 1, -20);
	glm::vec3 lookAtPosition = glm::vec3(0, 5, 0);
	glm::vec3 upDirection = glm::vec3(0, 1, 0);

	// view matrix: puts things relative to where the camera is
	glm::mat4 cameraView = glm::lookAt(
		eye, // eye, where the camera is
		lookAtPosition,  // look at position
		upDirection); // which way is up

	glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0, 1, 0));

	glm::mat4 cam_proj = glm::perspective(
		glm::radians(45.0f), // FOB
		640.0f / 480.0f,     // Aspect Ratio
		0.1f,                // Near Plane
		1000.0f);            // Far Plane

	Shader basicShad = aie::ReadShaderFromFiles("res/Shaders/CameraVertexShader.txt", "res/Shaders/TextureShader.frag");

	aie::Texture texture = aie::LoadTexture("res/Textures/soulspear_specular.tga");
	
	glm::vec3 ambientLight = glm::vec3(.6f, .6f, .6f);

	glm::vec3 sunlightDirection(0, 5, -1);
	glm::vec3 rot(0, 0, 0);

	while (!Window.ShouldClose())
	{
		Window.Tick();
		Window.Clear();

		rot += glm::vec3(0, 0.1f, 0);
		soulSpearTransform.SetEulerRotation(rot);
		soulSpearModel = soulSpearTransform.LocalMatrix();

		lookAtPosition = Window.GetCameraTransform()->LocalPosition;

		cameraView = glm::lookAt(
			eye,
			lookAtPosition,
			upDirection);

		// setup the all important uniforms
		aie::SetUniform(basicShad, 0, cam_proj);
		aie::SetUniform(basicShad, 1, cameraView);
		aie::SetUniform(basicShad, 2, soulSpearModel);

		aie::SetUniform(basicShad, 3, texture, 0);
		aie::SetUniform(basicShad, 4, ambientLight);
		aie::SetUniform(basicShad, 5, sunlightDirection);

		aie::Draw(basicShad, soulSpearGeometry);
	}
	
	aie::FreeShader(basicShad);
	aie::FreeGeometry(soulSpearGeometry);
	Window.Terminate();

	return 0;
}