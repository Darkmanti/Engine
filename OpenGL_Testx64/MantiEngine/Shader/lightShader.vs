#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

void main()
{
    gl_Position = projection * view * model * transform * vec4(position, 1.0f);
	FragPos = vec3(model * transform * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;
	TexCoord = vec2(texCoord.x, texCoord.y);
}
