#pragma once
#include "glm/glm.hpp"

struct GLFWwindow;
class Transform;

namespace aie
{
	class Context
	{
	public:
		GLFWwindow* Window;
		float CameraMovementSpeed;
		float CameraRotationSpeed;
	private:
		Transform* CameraTransform;
		glm::vec2 MousePosition;
		glm::vec2 MousePositionDifference;

		float ControlPitch = 0.0f;
		float ControlYaw = 0.0f;

	public:
		Transform* GetCameraTransform() { return CameraTransform; };

		bool Init(int Width, int Height, const char* Title);
		void Tick();
		void Clear();
		void Terminate();
		bool ShouldClose();

		void HandleInputs();
		bool IsKeyDown(int key);
		glm::vec2 GetMousePosition();
		float GetAxis(int negativeKey, int positiveKey);
	};
}