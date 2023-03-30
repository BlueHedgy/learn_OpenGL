#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "shader_class.h"
#include "camera_class.h"

#include "stb_image.h"


#include <vector>
#include <iostream>

// DEFAULT GLOBAL SETTINGS
// ----------------------------------------------------------------------------------------
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	float mixRatio = 0.2f;

	// Defining camera settings
	// -------------------------
	Camera camera1(glm::vec3(0.0f, 0.0f, 3.0f));
	float lastX = SCR_WIDTH / 2.0f;		// set camera/ cursor at center of the viewport
	float lastY = SCR_HEIGHT / 2.0f;

	bool firstMouse = true;


	// Setting up delta time for Camera speed syncing
	float deltaTime = 0.0f; // deltaTime =  currentFrameTime - lastFrameTime
	float lastFrame = 0.0f;

// -------------------------------------------------------------------------------------------
 

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	// To make the cursor's coord to be first default xy pos, 
	// prevent a camera jump due to cursor being far away from 
	// the center of the window at first interaction

	// safely convert double to float for x, y positions
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos; // reversed since y-coord is bottom to top (this should be user choice)
	lastX = xpos;
	lastY = ypos;

	camera1.processMouseInput(xOffset, yOffset, true);
}


void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	// This yOffset is that of the scrollwheel
	// not to be confused with the camera's yOffset

	camera1.processScrollInput(static_cast<float>(yOffset));
}

// input check

void processInput(GLFWwindow* window) {
	// check if Esc key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (mixRatio <= 1.0f) {
			mixRatio += 0.01f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (mixRatio >= 0.0f) {
			mixRatio -= 0.01f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		camera1.processKBInput(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		camera1.processKBInput(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		camera1.processKBInput(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		camera1.processKBInput(RIGHT, deltaTime);
	}
}

int main() {
	glfwInit();

	// Specifying openGl version to be used
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	

	//glfwWindonHint(GLFW_OPENGL_FOWARD_COMPAT, GL_TRUE);  MAC OS X ONLY

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learnOpenGL", NULL, NULL);

	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // render out the window (?)

	// set mouse look and capture cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// call back mouse calculation everytime the cursor moves
	glfwSetCursorPosCallback(window, mouse_callback);

	// call back mouse sroll every time we do so to zoom in or out
	glfwSetScrollCallback(window, scroll_callback);

	// call back mouse button
	//glfwSetMouseButtonCallback(window, mouse_button_callback);


	// call frame_buffer_size_callback every window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

// CONFIGURE GLOBAL OPENGL STATE
// ----------------------------------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);


// INITIALIZE SHADER AND RELEVANT OBJECTS
// ----------------------------------------------------------------------------------------------------
	Shader NewShader("3.3.Shader.vsh", "3.3.Shader.fsh");


	// Cube vertices
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

	// World space positions of the cubes
	std::vector<glm::vec3> cubePositions = {
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

	  
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};



	// copy our vertices array in a buffer for OpenGL to use
	// -----------------------------------------------------------
	unsigned int VBO;
	unsigned int VAO;
	//unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// somehow vertices.size()*sizeof(float) works
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices.front(), GL_STATIC_DRAW);
	

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// --- then set the vertex attributes pointers
	// --- position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


//  CREATING, ASSIGNING AND MAPPING TEXTURES TO VERTICES
// ------------------------------------------------------------------------------------------------------
	// --- declaring a texture 
	//GLuint* textures = new GLuint[2];
	unsigned int texture1, texture2;


	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// --- Load image using stb_image.h
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("img/container.jpg", &width, &height, &nrChannels, 0);

	// --- Generate the 2d texture
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Texture load unsuccesfully" << std::endl;
	}

	// --- free image memory
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	//glGenTextures(1, &texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	data = stbi_load("img/Wan.png", &width, &height, &nrChannels, 0);

	// Generate the 2d texture after checking valid input 
	// --------------------------------------------------
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Texture load unsuccesfully" << std::endl;
	}

	stbi_image_free(data);

	// create orthographic proj matrix
	// -----------------------------------------------------------------------------------
	//glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	//glm::mat4 proj = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);


	NewShader.use(); // don't forget to activate the shader before setting uniforms!  
	glUniform1i(glGetUniformLocation(NewShader.ID, "texture1"), 0); // set it manually
	NewShader.setInt("texture2", 1); // or with shader class



// RENDER LOOP, UPDATING CHANGES EVERY FRAME
// -------------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// rendering command
		// -----------------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// --- bind it so any config will affect this texture object
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);


		// Activate the shader 
		// -------------------
		NewShader.use();
		NewShader.setFloat("mixRatio", mixRatio);

	//-----------------------------------------------------------------------------------------------
	// RENDER MODIFICATIONS !!

		// Process delta time for camera syncing
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		
		// Obtain display matrices
		// -----------------------
		glm::mat4 viewMat = camera1.GetViewMatrix();
		glm::mat4 projectMat = glm::perspective(glm::radians(camera1.Zoom), (float)SCR_WIDTH/ (float)SCR_HEIGHT, 0.1f, 100.0f);

		// send matrices to shader
		// --------------------------------------------------------------------------------
		NewShader.setMat4("viewMat", viewMat);
		NewShader.setMat4("projectMat", projectMat);

	// ------------------------------------------------------------------------------------------
	// DRAW / RENDER THE OUTPUT

		glBindVertexArray(VAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // render in wireframe mode
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		for (int i = 0; i < cubePositions.size(); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);

			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			NewShader.setMat4("model", model);
				
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// check and call events, swap buffers
		// ----------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

