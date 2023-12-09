#include "Camera.h"

void Camera::MoveForward(float distance)
{
	position += forward * distance;
}

void Camera::MoveBackward(float distance)
{
	position -= forward * distance;
}

void Camera::MoveUp(float distance)
{
	position += worldUp * distance;
}

void Camera::MoveDown(float distance)
{
	position -= worldUp * distance;
}

void Camera::MoveLeft(float distance)
{
	position -= right * distance;
}

void Camera::MoveRight(float distance)
{
	position += right * distance;
}

void Camera::UpdateViewMatrix()
{
	view = glm::lookAt(position, position + forward, up);
}

void Camera::UpdateCameraVectors()
{
    // Calculate the new forward vector
    this->forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->forward.y = sin(glm::radians(pitch));
    this->forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->forward = glm::normalize(this->forward);
    // Also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(forward, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, forward));
}

void Camera::Set(const int width, const int height, const glm::vec3& position)
{
    this->isPerspective = true;
    this->yaw = YAW;
    this->pitch = PITCH;

    this->FoVy = FOV;
    this->width = width;
    this->height = height;
    this->zNear = zNEAR;
    this->zFar = zFAR;

    this->worldUp = glm::vec3(0, 1, 0);
    this->position = position;

    lastX = width / 2.0f;
    lastY = height / 2.0f;
    bFirstMouseMove = true;

    UpdateCameraVectors();
}

void Camera::Reset()
{
    position = startPosition;
    yaw = YAW;
    pitch = PITCH;
    UpdateCameraVectors();
}
