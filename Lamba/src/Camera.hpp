#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include<rxcpp/rx.hpp>
namespace Rx
{
	using namespace rxcpp;
	using namespace rxcpp::subjects;
	using namespace rxcpp::operators;
	using namespace rxcpp::util;
}
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
	FORWARD,
	BACKWARD ,
	LEFT,
	RIGHT 
};


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{

	Rx::observable<int> event_stream;
	Rx::observable <glm::vec<2, int>> mouse_observable;

public:
	
	Camera(Rx::observable<int> obs, Rx::observable < glm::vec<2, int> > mouse);
	glm::mat4 GetViewMatrix() const 
	{
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}
	void ProcessKeyboard(int direction);

	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

	void ProcessMouseScroll(float yOffset);
	
	float GetZoom()	{ return zoom; }

private:
	
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Eular Angles
	float yaw;
	float pitch;

	// Camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));  
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}


};