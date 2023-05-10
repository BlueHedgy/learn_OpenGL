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


// Default structures for Directional light
// --------------------------------------------------
struct DirLight{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};



// Default structures for Point Lights
// --------------------------------------------------
struct PointLight{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};



struct SpotLight{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse  = light.diffuse  * vec3(texture(material.diffuse, TexCoords))  * diff;
	vec3 specular = light.specular * vec3(texture(material.specular, TexCoords)) * spec;

	return (ambient + diffuse + specular);
}



vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos){
	vec3 lightDir = normalize(light.position - FragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*distance*distance);

	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse  = light.diffuse  * vec3(texture(material.diffuse, TexCoords))  * diff;
	vec3 specular = light.specular * vec3(texture(material.specular, TexCoords)) * spec;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos){

	vec3 lightDir = normalize(light.position - FragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*distance*distance);

	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse  = light.diffuse  * vec3(texture(material.diffuse, TexCoords))  * diff;
	vec3 specular = light.specular * vec3(texture(material.specular, TexCoords)) * spec;

	 // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}


// Initialize light uniforms
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

void main(){
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewcamPos - FragPos);

	vec3 result = calcDirLight(dirLight, normal, viewDir);

	for (int i = 0; i < NR_POINT_LIGHTS; i++){
		result += calcPointLight(pointLights[i], normal, viewDir, FragPos);
	}

	result += calcSpotLight(spotLight, normal, viewDir, FragPos);

	FragColor = vec4(result, 1.0);
}


