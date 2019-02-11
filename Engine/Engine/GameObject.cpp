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
	if (!V) delete[] V;
	if (!VT) delete[] VT;
	if (!F) delete[] F;
	if (!FT) delete[] FT;

	if (!shader) delete shader;
	if (!texture) delete texture;
}

GLuint &GameObject::GetVAO()
{
	return VAO;
}
GLuint &GameObject::GetVBO()
{
	return VBO;
}

GLuint &GameObject::GetEBO()
{
	return EBO;
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

float *GameObject::GetV()
{
	return V;
}

float *GameObject::GetVT()
{
	return VT;
}

int32_t *GameObject::GetF()
{
	return F;
}

int32_t *GameObject::GetFT()
{
	return FT;
}

void GameObject::SetV(float *v)
{
	V = v;
}

void GameObject::SetVT(float *vt)
{
	VT = vt;
}

void GameObject::SetF(int32_t *f)
{
	F = f;
}

void GameObject::SetFT(int32_t *ft)
{
	FT = ft;
}

void GameObject::Start()
{

}

void GameObject::Update()
{

}

void GameObject::Render()
{
	glBindVertexArray(VAO);

	glm::mat4 view = glm::mat4(1.0f);
	view = Engine::camera->GetViewMatrix();
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(Engine::camera->Zoom, (GLfloat)1024 / (GLfloat)1024, 0.1f, 1000.0f);

	shader->use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 0));
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::scale(trans, glm::vec3(20, 20, 20));
	shader->setUniform("model", model);
	shader->setUniform("transform", trans);

	// отправка в uniform матриц проекции и обзора
	shader->setUniform("view", view);
	shader->setUniform("projection", projection);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_TRIANGLES, countV, GL_UNSIGNED_INT, 0);

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
