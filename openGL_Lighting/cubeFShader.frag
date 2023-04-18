#version 330 core

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 viewcamPos;

struct Material{

	sampler2D diffuse;
	sampler2D specular;
	float	  shininess;
}; 

uniform Material material;

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;
               
void main(){
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

// CALCULATION FOR AMBIENT COLOR
// -----------------------------
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

// CALCULATION FOR DIFFUSE COLOR
// -----------------------------

	// diff is the cosine of angle between light direction to the fragment's position and its normal vector
	float diff = max(dot(normal, lightDir), 0.0);		// we use max to avoid negative values
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords))* lightColor);

// CALCULATION FOR SPECULAR HIGHLIGHT
// ---------------------------------
	vec3 viewDir = normalize(viewcamPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);

	// delta is the angle between camera view direction and the direction of the reflected light ray from lamp object(s)
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);  // number 32  is the fine level (shininess) of the highlight
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)) * lightColor);

// OUTPUT FRAGMENTS COLORS
// --------------------------------
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);


	
}