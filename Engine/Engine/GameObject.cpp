#include "GameObject.h"

GameObject::GameObject()
{
	enabled = true;
	shader = new Shader((Engine::dirAppData + "\\" + "Shaders\\shader.vs").c_str(), (Engine::dirAppData + "\\" + "Shaders\\shader.fs").c_str());

	countV = 0;
	countVT = 0;
	countF = 0;
	
	isMesh = false;
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

bool GameObject::IsMesh()
{
	return isMesh;
}

GLuint *GameObject::GetVAO()
{
	return &VAO;
}
GLuint *GameObject::GetVBO()
{
	return &VBO;
}

GLuint *GameObject::GetEBO()
{
	return &EBO;
}

int32_t *GameObject::GetCountV()
{
	return &countV;
}

int32_t *GameObject::GetCountVT()
{
	return &countVT;
}

int32_t *GameObject::GetCountF()
{
	return &countF;
}

float *GameObject::GetV()
{
	return V;
}

float *GameObject::GetVT()
{
	return VT;
}

GLuint *GameObject::GetF()
{
	return F;
}

GLuint *GameObject::GetFT()
{
	return FT;
}

void GameObject::SetV(float *v)
{
	if (!V) delete[] V;
	V = v;
}

void GameObject::SetVT(float *vt)
{
	if (!VT) delete[] VT;
	VT = vt;
}

void GameObject::SetF(GLuint *f)
{
	if (!F) delete[] F;
	F = f;
}

void GameObject::SetFT(GLuint *ft)
{
	if (!FT) delete[] FT;
	FT = ft;
}

void GameObject::Start()
{
	if (!enabled)
	{
		return;
	}
}

void GameObject::Update()
{
	if (!enabled)
	{
		return;
	}
}

void GameObject::Render(GLuint width, GLuint height)
{
	if (!enabled && !isMesh)
	{
		return;
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glm::mat4 view = glm::mat4(1.0f);
	view = Engine::camera->GetViewMatrix();
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(Engine::camera->Zoom, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
	shader->use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 0));
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::scale(trans, glm::vec3(2, 2, 2));
	shader->setUniform("model", model);
	shader->setUniform("transform", trans);

	// Отправка в uniform матриц проекции и обзора
	shader->setUniform("view", view);
	shader->setUniform("projection", projection);

	glDrawElements(GL_TRIANGLES, countF, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0); // Вроде как можно убрать
}

void GameObject::Enable()
{
	enabled = true;
}

void GameObject::Disable()
{
	enabled = false;
}
