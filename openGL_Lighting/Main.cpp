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

unsigned int loadImageTexture(char const* texturePath) {
	GLuint textureID;

	glGenTextures(1, &textureID);
	
	// load images with stbi
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture load unsuccesfully" << std::endl;
	}

	return textureID;
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Configure light attributes data
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Declaring and loading image texture maps
	GLuint diffuseMap = loadImageTexture("../img/container2.png");
	GLuint specularMap = loadImageTexture("../img/container2_specular.png");

	cubeShader.use();
	cubeShader.setInt("material.diffuse", 0);
	cubeShader.setInt("material.specular", 1);


	// Some preset values for the render loops
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	float specularStrength = 0.5;

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};


	
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
		glm::vec3 lightColor(1.0f);

		// RENDER MODIFICATIONS
		// ---------------------
	
		cubeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		cubeShader.setVec3("lightPos", lightPos);
                  
		// DEFAULT COLORS FOR ANY EMPTY OBJECT
		glm::vec3 defaultDiffuseColor = lightColor * glm::vec3(0.8f);
		glm::vec3 defaultAmbientColor = lightColor * glm::vec3(0.05f);
		glm::vec3 defaultSpecularColor = lightColor * glm::vec3(1.0f);

		// DEFAULT COEFFICIENTS FOR LIGHT ATTENUATION EQUATION
		GLfloat defaultConst	 = 1.0f;
		GLfloat defaultLinear	 = 0.09f;
		GLfloat defaultQuadratic = 0.032f;

		// Directional light variables
		cubeShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		cubeShader.setVec3("dirLight.ambient", defaultAmbientColor);
		cubeShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		cubeShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);


		// Point lights variables
		for (int i = 0; i < 4; i++) {
			cubeShader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);

			cubeShader.setVec3("pointLights[" + std::to_string(i) + "}.ambient", defaultAmbientColor);
			cubeShader.setVec3("pointLights[" + std::to_string(i) + "}.diffuse", defaultDiffuseColor);
			cubeShader.setVec3("pointLights[" + std::to_string(i) + "}.specular", defaultSpecularColor);

			cubeShader.setFloat("pointLights[" + std::to_string(i) + "].constant", defaultConst);
			cubeShader.setFloat("pointLights[" + std::to_string(i) + "].linear", defaultLinear);
			cubeShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", defaultQuadratic);
		}

		// Spot light variables
		cubeShader.setVec3("spotLight.position", camPerspective.Position);
		cubeShader.setVec3("spotLight.direction", camPerspective.Front);
		cubeShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		cubeShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		cubeShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

		cubeShader.setFloat("spotLight.constant", defaultConst);
		cubeShader.setFloat("spotLight.linear", defaultLinear);
		cubeShader.setFloat("spotLight.quadratic", defaultQuadratic);
		cubeShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		
		
		cubeShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		cubeShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		cubeShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		cubeShader.setFloat("material.shininess", 32.0f);
		
		
		// GET AND PASS DISPLAY MATRICES
		// --------------------
		glm::mat4 model			= glm::mat4(1.0f);
		glm::mat4 viewMat		= camPerspective.GetViewMatrix();
		glm::mat4 projectMat	= glm::perspective(glm::radians(camPerspective.Zoom), 
								  (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
		/*glm::mat3 normalMat		= glm::transpose(glm::inverse(model));*/


		//cubeShader.setMat4("model", model);
		cubeShader.setMat4("viewMat", viewMat);
		cubeShader.setMat4("projectMat", projectMat);
		cubeShader.setVec3("viewcamPos", camPerspective.Position); 

		//cubeShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f); // directional light 

		// DRAW / RENDER THE OUTPUT
		// ------------------------

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(cubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			cubeShader.setMat4("model", model);
			glm::mat3 normalMat = glm::transpose(glm::inverse(model));
			cubeShader.setMat3("normalMat", normalMat);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

	// ACTIVATE THE LAMP SHADER
	// ------------------------------------------------------
		lampShader.use();
		

		// RENDER MODIFICATIONS
		// ---------------------
		lampShader.setMat4("projectMat", projectMat);
		lampShader.setMat4("viewMat", viewMat);
		lampShader.setVec4("lampColor", glm::vec4(1.0));

		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));

			lampShader.setMat4("model", model);

			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		

	// Check and call events, swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}