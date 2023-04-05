#version 330 core

out vec4 FragColor;
uniform vec4 lampColor;

void main(){
	//FragColor = vec4(1.0);
	FragColor = lampColor;
}