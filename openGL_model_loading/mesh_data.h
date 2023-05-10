#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "shader_class.h"
#include "camera_class.h"
#include "primitive_cube.h"
#include "stb_image.h"


#ifndef MESH_DATA_H
#define	MESH_DATA_H
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
};

class Mesh {
public: 
	std::vector<Vertex>		vertices;
	std::vector<GLuint>		indices;
	std::vector<Texture>	textures;

	// constructor
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures) {
		this->vertices = vertices;
		this->textures = textures;
		this->indices = indices;

		setupMesh();
	}

	void Draw(Shader& shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

#endif