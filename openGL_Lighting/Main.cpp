#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "shader_class.h"
#include "camera_class.h"
#include "primitive_cube.h"
#include "stb_image.h"

#include <vector>
#include <iostream>

// DEFAULT GLOBAL SETTINGS
// -----------------------------------------------------------------------------------------------
	const GLuint SCR_WIDTH	= 800.0f;
	const GLuint SCR_HEIGHT = 600.0f;

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
	float yOffset = yPos - lastY;
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
}

int main() {
	glfwInit();

// SPECIFYING OPENGL VERSION FOR THE APPLICATION WINDOW
// ----------------------------------------------------
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


// CREATE THE APPLICATION WINDOW AND CALL FOR CALLBACK FUNCTIONS
	// Initialize the window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learnOpenGL_Lighting", NULL, NULL);


	// Error initialization check for GLAD and for the window
	// ------------------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	if (!window) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	
	// set mouse look and capture cursor, disable cursor visibility
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// call back window refresh when it is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// call back mouse input everytime the cursor is moved
	glfwSetCursorPosCallback(window, mouse_callback);

	// call back scroll input everytime the scrollwheel is used
	glfwSetScrollCallback(window, scroll_callback);



	// CONFIGURE OPENGL GLOBAL STATE
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

// INITIALIZE SHADER AND RELEVANT OBJECTS
	Shader ShaderPerspective("vertexShader.vsh", "fragmentShader.fhs");

	std::vector<float> vertices = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};


	// Create a new cube
	Cube Cube_1;
	std::vector<float> vertice_Cube_1 = Cube_1.Vertices;

// INITIALIZE AND CONFIGURE VBO, VAO, EBO
// ------------------------------------------------------------
	GLuint VBO;
	GLuint VAO;


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertice_Cube_1.size() * sizeof(float), &vertice_Cube_1.front(), GL_STATIC_DRAW);

	// Set vertex attributes pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// texture attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	return 0;
}