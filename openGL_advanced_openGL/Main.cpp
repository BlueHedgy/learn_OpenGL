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

GLuint loadTextureForBuffer(char const* texturePath) {
	GLuint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// attach texture buffer
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_HEIGHT, SCR_WIDTH, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

	// attach depth and stencil buffer
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SCR_HEIGHT, SCR_WIDTH, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);

	return textureID;
}

void enableStencilPass() {

}

void disableStencilPass() {

}

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
	//glDepthFunc(GL_LESS); // discard fragments further away

	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0XFF);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendEquation(GL_FUNC_ADD);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);

	glEnable(GL_MULTISAMPLE);

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	//stbi_set_flip_vertically_on_load(true);


// INITIALIZE SHADERS AND RELEVANT OBJECTS
// --------------------------------------------------------------------------------------------
	Cube cube1;
	std::vector<float> vertice_cube1 = cube1.Vertices;

	Plane plane1;
	std::vector<float> vertice_plane1 = plane1.Vertices;

	std::vector<glm::vec3> windowObj;
	windowObj.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	windowObj.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	windowObj.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	windowObj.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	windowObj.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	std::vector<float> transparentVertices = {
		// positions		// normals	// texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f, 0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f, 0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  0.0f, 0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f, 0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  0.0f, 0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  0.0f, 0.0f,  1.0f,  0.0f
	};

	std::vector<float> quadVertices= { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	Shader viewportShader("stencil_testing.vert", "stencil_testing.frag");	
	Shader viewportQuadShader("viewportQuad.vert", "viewportQuad.frag");
	//Shader objectOutline("stencil_testing.vert", "objectOutline.frag");


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

	// windowObj VA0
	GLuint windowObjVAO, windowObjVBO;
	glGenVertexArrays(1, &windowObjVAO);
	glGenBuffers(1, &windowObjVBO);
	
	glBindVertexArray(windowObjVAO);
	glBindBuffer(GL_ARRAY_BUFFER, windowObjVBO);
	glBufferData(GL_ARRAY_BUFFER, transparentVertices.size() * sizeof(float), &transparentVertices.front(), GL_STATIC_DRAW);


	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	// reserved normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	// texCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));

	glBindVertexArray(0);

	// screen quad VAO, VBO
	GLuint screenQuadVAO, screenQuadVBO;
	glGenVertexArrays(1, &screenQuadVAO);
	glGenBuffers(1, &screenQuadVBO);
	glBindVertexArray(screenQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), &quadVertices.front(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);


	// load textures
	GLuint cubeTexture = loadImageTexture("img/container.jpg");
	GLuint floorTexture = loadImageTexture("img/metal.png");
	GLuint grassTexture = loadImageTexture("img/grass.png");
	GLuint windowTexture = loadImageTexture("img/blending_transparent_window.png");

	viewportShader.use();
	viewportShader.setInt("texture1", 0);
	
	viewportQuadShader.use();
	viewportQuadShader.setInt("screenTexture", 0);


// FRAMEBUFFER CONFIG
// -----------------------------------------------------------------------------------
	// frame buffer object
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);


	// texture image as color attachment to framebuffer
	GLuint textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH*2/3, SCR_HEIGHT*2/3, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);


	// attach the texture the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureColorbuffer, 0);


	// render buffer object

	GLuint RBO;
	glGenRenderbuffers(1, &RBO);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // allocate memory for renderbuffer object
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer did NOT attach completely successfully";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind the framebuffer when finish using

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

// RENDER LOOP
// -------------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// bind to target framebuffer and draw scene normally to colortexturebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);

		// clear framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		

		// rendering SCENE
		
		

		// Activate the shader(s)
		viewportShader.use();

		glm::mat4 model(1.0f);
		glm::mat4 projectMat = glm::perspective(glm::radians(camPerspective.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 viewMat = camPerspective.GetViewMatrix();

		viewportShader.setMat4("view", viewMat);
		viewportShader.setMat4("projection", projectMat);
		

	/*	objectOutline.use();
		objectOutline.setMat4("view", viewMat);
		objectOutline.setMat4("projection", projectMat);*/


		// draw objects 
		// ---------------------------------------------------

		// draw floor
		//glStencilMask(0x00);

		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		viewportShader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		

		// Draw cubes
		
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);

		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f)); 
		viewportShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		viewportShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		// Rebind to default framebuffer and draw the viewportQuad
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // so viewportQuad can't be discarded due to depth test

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		viewportQuadShader.use();
		glBindVertexArray(screenQuadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//// Draw outlines
		//// ------------------------------------------------------
		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);
		//objectOutline.use();
		//float scale = 1.01f;

		//glBindVertexArray(cubeVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, cubeTexture);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		//model = glm::scale(model, glm::vec3(scale, scale, scale));
		//objectOutline.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(scale, scale, scale));
		//objectOutline.setMat4("model", model);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

		//glStencilMask(0xFF);
		//glStencilFunc(GL_ALWAYS, 0, 0xFF);
		//glEnable(GL_DEPTH_TEST);
		//
		// draw windowObj
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);
		//

		//glBindVertexArray(windowObjVAO);
		//glBindTexture(GL_TEXTURE_2D,windowTexture);

		//std::map<float, glm::vec3> sortedObjByDist;
		//for (unsigned int i = 0; i < windowObj.size(); i++) {
		//	float dist = glm::length(camPerspective.Position - windowObj[i]);
		//	sortedObjByDist[dist] = windowObj[i];
		//}

		//std::map<float, glm::vec3>::reverse_iterator it;
		//for (it = sortedObjByDist.rbegin(); it != sortedObjByDist.rend(); it++) {
		//	model = glm::mat4(1.0f);
		//	model = glm::translate(model, it->second);
		//	viewportShader.setMat4("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}


		// Check and call events, swap buffers*
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
} 