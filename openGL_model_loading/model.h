#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "shader_class.h"
#include "camera_class.h"
#include "stb_image.h"

#include "mesh_data.h"

// Assimp library for loading models
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
	// constructor
	std::vector<Mesh> meshes_list;
	std::string directory;
	std::vector<Texture> textures_loaded;
	bool gammaCorrection;

	Model(std::string const &path, bool gamma = false) : gammaCorrection(gamma) {
		loadModel(path);
	}
	void Draw(Shader& shader);

private:
	
	void loadModel(std::string const &path);
	void processNode(aiNode* mode, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

};

