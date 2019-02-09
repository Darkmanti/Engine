#version 330 core

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 positionFlashLight;
	vec3 direction;
	float cutOff;
	float outerCutOff;
};

uniform Light light;
uniform Material material;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 color;

uniform vec3 viewPos;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.positionFlashLight - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	
	if(theta > light.outerCutOff)
	{
		// ambient
		vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
	
		// diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(light.positionFlashLight - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
	
		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;
		
		// spotlight
		float theta = dot(lightDir, normalize(-light.direction));
		float epsilon   = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
		
		diffuse  *= intensity;
		specular *= intensity;
	
		// attenuation
		float distance = length(light.positionFlashLight - FragPos);
		float attenuation = 1.0/(light.constant + light.linear * distance +
		light.quadratic * (distance * distance));
	
		ambient  *= attenuation; 
		diffuse  *= attenuation;
		specular *= attenuation;

		vec3 result = ambient + diffuse + specular;
		color = vec4(result, 1.0f);
	}else{
	
	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
	
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
	
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;
	
	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0/(light.constant + light.linear * distance +
	light.quadratic * (distance * distance));
	
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);}
}