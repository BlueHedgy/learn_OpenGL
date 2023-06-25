#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

float near = 5.0;
float far = 100.0;

float linearizeDepth(float depth)
{
	float ndc = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - ndc * (far - near));
}

void main(){
//	FragColor = texture(texture1, TexCoords);
	float depth = linearizeDepth(gl_FragCoord.z) / far;
	FragColor = texture(texture1, TexCoords) * vec4(vec3(depth), 1.0);
}














