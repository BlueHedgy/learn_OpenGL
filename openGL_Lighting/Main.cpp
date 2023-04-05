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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learnOpenGL_Lighting", NULL, NULL);


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



// INITIALIZE SHADERS AND RELEVANT OBJECTS
	Shader cubeShader("cubeVShader.vert", "cubeFShader.frag");
	Shader lampShader("lightVShader.vert", "lightFShader.frag");


	// Create a new cube
	Cube Cube_1;
	std::vector<float> vertice_Cube_1 = Cube_1.Vertices;

// INITIALIZE AND CONFIGURE VBO, VAO, EBO
// ------------------------------------------------------------
	GLuint VBO;
	GLuint cubeVAO, lightVAO;

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertice_Cube_1.size() * sizeof(float), &vertice_Cube_1.front(), GL_STATIC_DRAW);

	// Set vertex attributes pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Configure light attributes data
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Some preset values for the render loops
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	float specularStrength = 0.5;
	
// RENDER LOOP
// -------------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// rendering commands
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

	// ACTIVATE THE CUBE SHADER
	// ------------------------------------------------------------
		cubeShader.use();

		// RENDER MODIFICATIONS
		// ---------------------
	
		cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		cubeShader.setVec3("lightPos", lightPos);


		// GET DISPLAY MATRICES
		// --------------------
		glm::mat4 model			= glm::mat4(1.0f);
		glm::mat4 viewMat		= camPerspective.GetViewMatrix();
		glm::mat4 projectMat	= glm::perspective(glm::radians(camPerspective.Zoom), 
								  (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat3 normalMat		= glm::transpose(glm::inverse(model));


		cubeShader.setMat4("model", model);
		cubeShader.setMat4("viewMat", viewMat);
		cubeShader.setMat4("projectMat", projectMat);
		cubeShader.setMat3("normalMat", normalMat);
		cubeShader.setVec3("viewcamPos", camPerspective.Position); 
		cubeShader.setFloat("specularStrength", specularStrength);

		// DRAW / RENDER THE OUTPUT
		// ------------------------
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

	// ACTIVATE THE LIGHT SHADER
	// ------------------------------------------------------
		lampShader.use();
		

		// RENDER MODIFICATIONS
		// ---------------------
		
		glm::vec4 lampColor(1.0f, 1.0f, (float)sin(glfwGetTime())*1.0f, 1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));


		lampShader.setMat4("model", model);
		lampShader.setMat4("projectMat", projectMat);
		lampShader.setMat4("viewMat", viewMat);
		lampShader.setVec4("lampColor", lampColor);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


	// Check and call events, swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}