#include "camera_class.h"

// return view matrix from every updated Euler angles and lookat matrix
glm::mat4 Camera::GetViewMatrix() {
	// Defining LookAt Matrix into viewmat
		// -------------------------------
		// cameraPos + cameraFront here just means it's looking at (0,1,0),
		// indicate direction same usage with cameraUp here, just to point 
		// in up direction for the camera 
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::updateCameraVectors(){
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front	= glm::normalize(front);

	Right	= glm::normalize(glm::cross(Front, WorldUp));
	Up		= glm::normalize(glm::cross(Right, Front));
}

// general input processing from any keyboard like system
void Camera::processKBInput(CameraMovement direction, float deltaTime) {
	float velocity = CameraSpeed * deltaTime * 3.0;
	if (direction == FORWARD) {
		Position += Front * velocity;
	}
	if (direction == BACKWARD) {
		Position -= Front * velocity;
	}
	if (direction == LEFT) {
		Position -= Right * velocity;
	}
	if (direction == RIGHT) {
		Position += Right * velocity;
	}
}

// general input processing for mouse like system, expect offset in both x and y direction
void Camera::processMouseInput(float xOffset, float yOffset, GLboolean constraintPitch = true) {
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	if (constraintPitch == true) {
		if (Pitch < 89.0f || Pitch > -89.0f) {
			Pitch += yOffset;
		}
	}
	updateCameraVectors();
}

// process input from scrollwheel, only expect offset in y direction
void Camera::processScrollInput(float yOffset) {
	if (Zoom > 1.0f || Zoom < 45.0f) {
		Zoom -= 3 * yOffset;
	}
}