#pragma once

#include "Shader.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Importer.h"

class GameObject
{
public:

	GLuint		VAO, VBO, EBO;

	uint64_t	Vcount, Fcount;

	Shader		*shader, *selectShader;

	GLuint		texture;

	glm::mat4	model;

	bool		isSelect;

	//Script*		scripts;

	GameObject(Shader* shader_, Shader* selectShader_, char const* fileName, GLuint texture_);

	GameObject(GLfloat* vertices_, uint64_t Vcount_, Shader* shader_, GLuint texture_);

	void setModel(glm::vec3 trans_, glm::vec3 model_, GLfloat degree_, glm::vec3 axis_);

	void DrawArray(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos);

	void DrawArray_temp(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos);

	void DrawElement(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos);
};

