#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "shader_class.h"
#include "camera_class.h"
#include "stb_image.h"
#include "model.h"
#include "primitive_cube.h"
#include "primitive_plane.h"

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


unsigned int loadImageTexture(char const* texturePath) {
	GLuint textureID;

	glGenTextures(1, &textureID);

	// load images with stbi
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	// --- Generate the 2d texture
	if (data) {
		GLenum format{};
		if (nrChannels == 1) format = GL_RED;
		else if (nrChannels == 3) format = GL_RGB;
		else if (nrChannels == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture load unsuccesfully" << std::endl;
	}

	return textureID;
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

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// CREATE THE APPLICATION WINDOW AND CALL FOR CALLBACK FUNCTIONS
		// Initialize the window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learnOpenGL_advanced_openGL", NULL, NULL);


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
	glDepthFunc(GL_LESS); // discard fragments further away

	glEnable(GL_MULTISAMPLE);

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	//stbi_set_flip_vertically_on_load(true);


	// INITIALIZE SHADERS AND RELEVANT OBJECTS
	Cube cube1;
	std::vector<float> vertice_cube1 = cube1.Vertices;

	Plane plane1;
	std::vector<float> vertice_plane1 = plane1.Vertices;

	Shader ourShader("depth_testing.vert", "depth_testing.frag");

	// Cube VAO
	// -------------------------------------------------------------------------------------------
	GLuint cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, vertice_cube1.size() * sizeof(float), &vertice_cube1.front(), GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// texCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// unbind vertex array
	glBindVertexArray(0);


	// Plane VAO
	// --------------------------------------------------------------------------------------------
	GLuint planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, vertice_plane1.size() * sizeof(float), &vertice_plane1.front(), GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// reserved normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// texCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);


	// load textures
	GLuint cubeTexture = loadImageTexture("img/marble.jpg");
	GLuint floorTexture = loadImageTexture("img/metal.png");

	ourShader.use();
	ourShader.setInt("texture1", 0);

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

		glm::mat4 model(1.0f);
		glm::mat4 projectMat = glm::perspective(glm::radians(camPerspective.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 viewMat = camPerspective.GetViewMatrix();

		ourShader.setMat4("projection", projectMat);
		ourShader.setMat4("view", viewMat);

		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f)); 
		ourShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		ourShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// floor
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		ourShader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
			
		// Check and call events, swap buffers*
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}