#include "Camera.h"
#include <glm\gtx\transform.hpp>

const float Camera::MOVEMENT_SPEED = 0.1f;
int x_sign, z_sign;

Camera::Camera() : 
	viewDirection(0.009740635f, -0.592421985f, 0.862995136f),
	position(13.0f, 5.23070025f, 6.0f),
	UP(0.0f, 1.0f, 0.0f)
{
}

void Camera::mouseUpdate(const glm::vec2& newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	if (glm::length(mouseDelta) > 50.0f)
	{
		oldMousePosition = newMousePosition;
		return;
	}
	const float ROTATIONAL_SPEED = 0.5f;
	strafeDirection = glm::cross(viewDirection, UP);
	glm::mat4 rotator =	glm::rotate(-mouseDelta.x * ROTATIONAL_SPEED, UP) *
						glm::rotate(-mouseDelta.y * ROTATIONAL_SPEED, strafeDirection);
	
	viewDirection = glm::mat3(rotator) * viewDirection;

	oldMousePosition = newMousePosition;
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, UP);
}

void Camera::moveForward()
{
	if(abs(position.x) < 15 && abs(position.z) < 15)
		position += MOVEMENT_SPEED * viewDirection;
	else {
		x_sign = position.x / abs(position.x);
		z_sign = position.z / abs(position.z);

		position.x = (abs(position.x) - 1) * x_sign;
		position.z = (abs(position.z) - 1) * z_sign;

	}
}

void Camera::moveBackward()
{
	if (abs(position.x) < 15 && abs(position.z) < 15)
		position += -MOVEMENT_SPEED * viewDirection;
	else {
		x_sign = position.x / abs(position.x);
		z_sign = position.z / abs(position.z);

		position.x = (abs(position.x) - 1) * x_sign;
		position.z = (abs(position.z) - 1) * z_sign;

	}
}

void Camera::strafeLeft()
{
	//printf("%f\t%f\t%f\n", position.x, position.y, position.z);

	if (abs(position.x) < 15 && abs(position.z) < 15)
		position += -MOVEMENT_SPEED * strafeDirection;
	else {
		x_sign = position.x / abs(position.x);
		z_sign = position.z / abs(position.z);

		position.x = (abs(position.x) - 1) * x_sign;
		position.z = (abs(position.z) - 1) * z_sign;

	}
}

void Camera::strafeRight()
{
	//printf("%f\t%f\t%f\n", position.x, position.y, position.z);

	if (abs(position.x) < 15 && abs(position.z) < 15)
		position += MOVEMENT_SPEED * strafeDirection;
	else {
		x_sign = position.x / abs(position.x);
		z_sign = position.z / abs(position.z);

		position.x = (abs(position.x) - 1) * x_sign;
		position.z = (abs(position.z) - 1) * z_sign;

	}
}

void Camera::moveUp()
{
	if(position.y < 10 )
		position += MOVEMENT_SPEED * UP;
}

void Camera::moveDown()
{
	if (position.y > 1)
		position += -MOVEMENT_SPEED * UP;
}