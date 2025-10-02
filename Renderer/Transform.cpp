#include "Transform.h"

Transform::Transform()
{
    LocalPosition = { 0, 0, 0 };
    LocalRotation = glm::quat();
    LocalScale = { 1, 1, 1 };
}

glm::mat4 Transform::LocalMatrix() const
{
    glm::mat4 localMatrix = glm::translate(glm::mat4(1.0f), LocalPosition);
    localMatrix *= glm::toMat4(LocalRotation);
    localMatrix = glm::scale(localMatrix, LocalScale);
    return localMatrix;
}

glm::quat Transform::SetEulerRotation(glm::vec3 eulerAngles)
{
    LocalRotation = glm::quat(eulerAngles);

    return LocalRotation;
}

glm::quat Transform::AddEulerRotation(glm::vec3 eulerAngles)
{
    LocalRotation *= glm::quat(eulerAngles);

    return LocalRotation;
}
