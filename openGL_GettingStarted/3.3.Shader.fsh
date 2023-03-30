// define fragment shader (compute colors)
#version 330 core

out vec4 FragColor;

// same name as in vertex shader => linked
 
in vec2 TexCoord;

uniform float mixRatio;

uniform sampler2D texture1;
uniform sampler2D texture2; // add second shader



void main(){

	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixRatio);
}