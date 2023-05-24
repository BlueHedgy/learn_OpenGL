#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "shader_class.h"
#include "camera_class.h"
#include "primitive_cube.h"
#include "stb_image.h"

#define MAX_BONE_INFLUENCE 4



#ifndef MESH_DATA_H
#define	MESH_DATA_H
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	//// tangent
	//glm::vec3 Tangent;
	//// bitangent
	//glm::vec3 Bitangent;
	////bone indexes which will influence this vertex
	//int m_BoneIDs[MAX_BONE_INFLUENCE];
	////weights from each bone
	//float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public: 
	std::vector<Vertex>		vertices;
	std::vector<unsigned int>		indices;
	std::vector<Texture>	textures;


	// constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
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