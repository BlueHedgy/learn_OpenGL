#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out vec3 Normal;		// object face normal (local normal)
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat3 normalMat;
uniform mat4 viewMat;
uniform mat4 projectMat;


void main() {
	gl_Position = projectMat * viewMat * model * vec4(aPos, 1.0);

	FragPos = vec3(model * vec4(aPos, 1.0));		//world space coordinate of the fragment
	Normal = normalMat * aNormal;					// multiply notmal matrix with world space Normal (aNormal) to 
													// prevent crooked light shading on non-uniform scal

	TexCoords = aTexCoords;
}