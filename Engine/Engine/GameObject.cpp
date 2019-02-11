#include "GameObject.h"

GameObject::GameObject()
{
	enabled = true;
	shader = new Shader("Shader/shader.vs", "Shader/shader.fs");

	countV = 0;
	countVT = 0;
	countF = 0;
}

GameObject::~GameObject()
{
	delete shader;
	delete texture;
}

int32_t &GameObject::GetCountV()
{
	return countV;
}

int32_t &GameObject::GetCountVT()
{
	return countVT;
}

int32_t &GameObject::GetCountF()
{
	return countF;
}

float *GetV()
{

}

float *GetVT()
{

}

int32_t *GetF()
{

}

int32_t *GetFT()
{

}

void GameObject::Render()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glGenBuffers(1, &EBO);

	glBufferData(GL_ARRAY_BUFFER, countV * sizeof(float), v, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, countF / 2 * sizeof(float), fv, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void GameObject::Enable()
{
	enabled = true;
}

void GameObject::Disable()
{
	enabled = false;
}
