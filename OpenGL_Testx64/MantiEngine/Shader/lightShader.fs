#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 color;

uniform vec3 viewPos;

uniform sampler2D ourTexture1;

void main()
{
	// ambient
	vec3 ambient = light.ambient * material.ambient;
	
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);
	
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);
	
	vec3 result = (ambient + diffuse + specular) * vec3(texture(ourTexture1, TexCoord));
    color = vec4(result, 1.0f);
}