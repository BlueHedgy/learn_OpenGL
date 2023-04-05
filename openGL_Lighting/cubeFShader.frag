#version 330 core

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

uniform float specularStrength;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 viewcamPos;


void main(){
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

// CALCULATION FOR AMBIENT COLOR
// -----------------------------
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

// CALCULATION FOR DIFFUSE COLOR
// -----------------------------

	// theta is the angle between light direction to the fragment's position and its normal vector
	float theta = max(dot(normal, lightDir), 0.0);		// we use max to avoid negative values
	vec3 diffuse = theta * lightColor;

// CALCULATION FOR SPECULAR HIGHLIGHT
// ---------------------------------
	vec3 viewDir = normalize(viewcamPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);

	// delta is the angle between camera view direction and the direction of the reflected light ray from lamp object(s)
	float delta = pow(max(dot(viewDir, reflectDir), 0.0), 64);  // number 32  is the fine level (shininess) of the highlight
	vec3 specular = specularStrength * delta * lightColor;

// OUTPUT FRAGMENTS COLORS
// --------------------------------
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);

	
}