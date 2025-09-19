#include "Transform.h"

Transform::Transform()
{
}

glm::mat4 Transform::LocalMatrix() const
{
    glm::mat4 localMatrix = glm::translate(glm::mat4(1.0f), LocalPosition);
    localMatrix *= glm::toMat4(LocalRotation);
    localMatrix = glm::scale(localMatrix, LocalScale);
    return localMatrix;
}
