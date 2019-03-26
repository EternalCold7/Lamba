#include "Camera.hpp"
#include <GLFW/glfw3.h>
// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVTY = 0.25f;
const float ZOOM = 45.0f;


Camera::Camera(Rx::observable<int> stream, Rx::observable < glm::vec<2, int> > mouse) :
	front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY),
	zoom(ZOOM), position(glm::vec3(0.0f, 0.0f, 3.0f)), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	yaw(YAW), pitch(PITCH)
{
	event_stream = stream;
	event_stream.filter([](int a) {
		return a == GLFW_KEY_UP || a == GLFW_KEY_RIGHT || a == GLFW_KEY_LEFT || a == GLFW_KEY_DOWN;
	}).subscribe([this](int val) { ProcessKeyboard(val); });
	mouse_observable = mouse;

	mouse_observable.subscribe([this](glm::vec<2, int> vec) {
		ProcessMouseMovement(vec.x, vec.y);
	});
}

void Camera::ProcessKeyboard(int dir)
{
	float velocity = movementSpeed;
	Camera_Movement direction = dir == GLFW_KEY_UP ? FORWARD :
		dir == GLFW_KEY_DOWN ? BACKWARD : dir == GLFW_KEY_LEFT ? LEFT : RIGHT;

	if (direction == FORWARD || direction == BACKWARD)
		position += (direction == FORWARD ? front : -front) * velocity;
	else
		position += (direction == LEFT ? -right : right) * velocity;
}



void Camera::ProcessMouseScroll(float yOffset)
{
	if (this->zoom >= 1.0f && this->zoom <= 45.0f)
		this->zoom -= yOffset;

	if (this->zoom <= 1.0f)
		this->zoom = 1.0f;

	if (this->zoom >= 45.0f)
		this->zoom = 45.0f;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (this->pitch > 89.0f)
			pitch = 89.0f;
		
		if (this->pitch < -89.0f)
			this->pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	this->updateCameraVectors();
}