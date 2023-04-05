#version 330 core
layout (location = 0) in vec3 aPos;


uniform mat4 model;
uniform mat4 viewMat;
uniform mat4 projectMat;


void main() {
	gl_Position = projectMat * viewMat * model * vec4(aPos, 1.0);
}