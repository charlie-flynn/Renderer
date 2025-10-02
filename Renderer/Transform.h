#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

class Object;

class Transform
{
public:
	glm::vec3 LocalPosition;
	glm::quat LocalRotation;
	glm::vec3 LocalScale;

	Transform();

	glm::mat4 LocalMatrix() const;

	glm::quat SetEulerRotation(glm::vec3 eulerAngles);
	glm::quat AddEulerRotation(glm::vec3 eulerAngles);
};