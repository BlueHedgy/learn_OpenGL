#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "shader_class.h"
#include "camera_class.h"
#include "stb_image.h"
#include "model.h"

#include <vector>
#include <iostream>

// DEFAULT GLOBAL VIEWPORT SETTINGS
// -----------------------------------------------------------------------------------------------
const unsigned int SCR_WIDTH = 800.0f;
const unsigned int SCR_HEIGHT = 600.0f;

// default cameras settings
Camera camPerspective(glm::vec3(0.0f, 0.0f, 3.0f));

// default cursor position (viewport center)
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// 
bool firstMouse = true;

// Setting up delta time for camera speed syncing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// -----------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn) {
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	camPerspective.processMouseInput(xOffset, yOffset, true);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	// This yOffset is that of the scrollwheel
	// not to be confused with the camera's yOffset

	camPerspective.processScrollInput(static_cast<float>(yOffset));
}


// Input check

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camPerspective.processKBInput(FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camPerspective.processKBInput(LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camPerspective.processKBInput(BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camPerspective.processKBInput(RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camPerspective.processKBInput(ASCEND, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		camPerspective.processKBInput(DESCEND, deltaTime);
	}
}


//void userCommand() {
//	std::string inCommand;
//	std::cout << "Please enter your command from the avaiable list: " << std::endl;
//	std::cin >> inCommand;
//}


int main() {
	glfwInit();

	// SPECIFYING OPENGL VERSION FOR THE APPLICATION WINDOW
	// ----------------------------------------------------
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);



	// CREATE THE APPLICATION WINDOW AND CALL FOR CALLBACK FUNCTIONS
		// Initialize the window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learnOpenGL_model_loading", NULL, NULL);


	// Error initialization check for GLAD and for the window
	// ------------------------------------------------------

	if (!window) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // render out the window (?)

	// set mouse look and capture cursor, disable cursor visibility
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// call back window refresh when it is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// call back mouse input everytime the cursor is moved
	glfwSetCursorPosCallback(window, mouse_callback);

	// call back scroll input everytime the scrollwheel is used
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// CONFIGURE OPENGL GLOBAL STATE
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	//stbi_set_flip_vertically_on_load(true);


	// INITIALIZE SHADERS AND RELEVANT OBJECTS
	//std::string object_path = "backpack/backpack.obj";
	//std::string object_path = "D:/3D projects/Bows and Arrows/Bow models.obj";
	std::string object_path = "G:/Visual Studio Project/microphone object/untitled.obj";
	Model ourModel(object_path);
	Shader ourShader("model_vShader.vert", "model_fShader.frag");

	// RENDER LOOP
	// -------------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// rendering commands
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// Activate the shader
		ourShader.use();


		glm::mat4 projectMat = glm::perspective(glm::radians(camPerspective.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 viewMat = camPerspective.GetViewMatrix();

		ourShader.setMat4("projection", projectMat);
		ourShader.setMat4("view", viewMat);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		// Check and call events, swap buffers*
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}