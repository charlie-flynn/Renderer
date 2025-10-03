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

// #define USE_DEBUG_SPHERE

int main()
{
	Context Window;
	Window.Init(640, 480, "my awesome window <3");
	Window.CameraMovementSpeed = 1.0f;
	Window.CameraRotationSpeed = 1.0f;

#ifdef USE_DEBUG_SPHERE
	Geometry soulSpearGeometry = aie::LoadGeometry("res/Models/uv-sphere.obj");
#else
	Geometry soulSpearGeometry = aie::LoadGeometry("res/Models/soulspear.obj");
#endif


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

#ifdef USE_DEBUG_SPHERE
	aie::Texture texture = aie::LoadTexture("res/Textures/baby anteater.jpg");
#else
	aie::Texture texture = aie::LoadTexture("res/Textures/soulspear_specular.tga");
#endif
	
	glm::vec3 ambientLight = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 sunlightDirection(0, 5, 0);
	glm::vec3 sunlightColor(1.0f, 0.0f, 0.0f);

	glm::vec3 secondLightDirection(-5, 0, 0);
	glm::vec3 secondLightColor(0.0f, 1.0f, 0.0f);

	soulSpearTransform.LocalPosition.z = -8;


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
		aie::SetUniform(basicShad, 6, secondLightDirection);
		aie::SetUniform(basicShad, 7, sunlightColor);
		aie::SetUniform(basicShad, 8, secondLightColor);

		aie::Draw(basicShad, soulSpearGeometry);
	}
	
	aie::FreeShader(basicShad);
	aie::FreeGeometry(soulSpearGeometry);
	Window.Terminate();

	return 0;
}