#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum CameraMovement{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// default camera Attributes
const float YAW			= -90.0f;
const float PITCH		= 0.0f;
const float SPEED		= 3.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM		= 45.0f;

class Camera {

public:
	// Default camera position Attributes
	// ----------------------------------
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float CameraSpeed;
	float MouseSensitivity;
	float Zoom;

	// Setting up delta time for Camera speed syncing
	float deltaTime = 0.0f; // deltaTime =  currentFrameTime - lastFrameTime
	float lastFrame = 0.0f;

	// constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) :
		Front(glm::vec3(0.0f, 0.0f, -1.0f)), CameraSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// constructor with scalar value
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		yaw = yaw;
		pitch = pitch;
		updateCameraVectors();
	}
	
	// processing input functions
	glm::mat4 GetViewMatrix();
	void processKBInput(CameraMovement direction, float deltaTime);
	void processMouseInput(float xOffset, float yOffset, GLboolean constraintPitch);
	void processScrollInput(float yOffset);

private:
	// calculate the front vector each time the Euler angles change
	void updateCameraVectors();
};
#endif // !CAMERA_H\
