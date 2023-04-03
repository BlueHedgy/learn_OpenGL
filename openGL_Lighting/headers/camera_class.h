#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


// Camera movement variables
enum cameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


// default Camere attributes
const float YAW			= -90.0f;
const float PITCH		= 0.0f;
const float SPEED		= 3.0f;
const float SENSITIVITY = 1.0f;
const float FOV			= 45.0f;


class Camera {
public:

	// Camera Attributes variables
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler angles
	float Yaw;
	float Pitch;

	// camera options
	float CameraSpeed;
	float MouseSensitivity;
	float Zoom;


	// Constructor
	Camera(	glm::vec3 position	= glm::vec3(0.0f, 0.0f, 3.0f),
			glm::vec3 up		= glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw			= YAW,
			float pitch			= PITCH)
			:
			Front(glm::vec3(0.0f, 0.0f, -1.0f)),
			CameraSpeed(SPEED),
			Zoom(FOV)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix();
	void processKBInput(cameraMovement direction, float deltaTime);
	void processMouseInput(float xOffset, float yOffset, GLboolean constraintPitch);
	void processScrollInput(float yOffset);

private:

	void updateCameraVectors();

};
#endif