#pragma once

#include "Shader.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

class GameObject
{
public:

	GLfloat*	vertices;

	GLuint		VAO, VBO, EBO;

	uint64_t	Vcount, Fcount;

	Shader*		shader;

	GLuint		texture;

	glm::mat4	model;

	GameObject(GLuint texture);

	GameObject(GLfloat* vertices_, uint64_t Vcount_, Shader* shader_, GLuint texture_);

	void Init(GLfloat* vertices_, uint64_t Vcount_, GLfloat* Faces_, uint64_t Fcount_, Shader* shader_, GLuint texture_);

	void setModel(glm::vec3 trans_, glm::vec3 model_, GLfloat degree_, glm::vec3 axis_);

	void DrawArray(glm::mat4 projection, glm::mat4 view);

	void DrawElement(glm::mat4 projection, glm::mat4 view);
};

