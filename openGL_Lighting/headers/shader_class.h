#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include "iostream"


class Shader {
public:

	GLuint ID; //shader ID

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {   
		std::string vertexCode, fragmentCode;	
		std::ifstream vShaderFile, fShaderFile;

		// try and perform checksum on shader files
		vShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

		// read the shader files as streams
		try {
			vShaderFile.open(vertexShaderPath);
			fShaderFile.open(fragmentShaderPath);
			std::stringstream vShaderStream, fShaderStream;

			// read the buffer of shader files into stringstreams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// close the files after reading
			vShaderFile.close();
			fShaderFile.close();

			// convert the code file to strings
			vertexCode		= vShaderStream.str();
			fragmentCode	= fShaderStream.str();
		}
		
		catch(std::ifstream::failure error ){
			std::cout << "Failed to read shader file" << error.what() << std::endl;
		}

		// convert the code strings to C- strings
		const char* vShaderCode = vertexCode.c_str();
		 const char* fShaderCode = fragmentCode.c_str();

		// Create vertex and fragment shader from definition C-string code
		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		compileCheck(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		compileCheck(fragment, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		compileCheck(ID, "PROGRAM");


		// Delete shaders after linking to free up memory
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	void use() {
		glUseProgram(ID);
	}

	// set Values for variables in vertex and fragment shaders

	void setBool(std::string &name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
	}

	void setInt(std::string &name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	// set values for uniform in vertex and fragmenmt shaders
	void setVec2(const std::string& name, const glm::vec2& value) const {
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) const {
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w) const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const {
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const {
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	void compileCheck(GLuint shader, std::string type) {
		int success;
		char infoLog[1024];

		if (type != "PROGRAM") {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR COMPILING SHADER, TYPE :" << type << std::endl;
		}
		else {
			glGetShaderInfoLog(ID, 1024, NULL, infoLog);
			std::cout << "ERROR LINKING SHADE, TYPE :" << type << infoLog << std::endl;
		}
	}
};

#endif // !SHADER_H
