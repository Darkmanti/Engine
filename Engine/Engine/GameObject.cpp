#include "GameObject.h"

GameObject::GameObject(Shader* shader_, char const* fileName, GLuint texture_)
{
	shader = shader_;
	texture = texture_;

	Vcount = 0; Fcount = 0;
	Importer::ImportObj(fileName, VAO, VBO, EBO, Vcount, Fcount);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
}

GameObject::GameObject(GLfloat* vertices_, uint64_t Vcount_, Shader* shader_, GLuint texture_)
{
	shader = shader_;
	texture = texture_;
	Vcount = Vcount_;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vcount * sizeof(vertices_), vertices_, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
}

void GameObject::setModel(glm::vec3 trans_, glm::vec3 model_, GLfloat degree_, glm::vec3 axis_)
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, model_);
	model = glm::rotate(model, degree_, axis_);
	model = glm::scale(model, trans_);
}

void GameObject::DrawArray(glm::mat4 projection, glm::mat4 view)
{
	shader->use();
	shader->setUniform("projection", projection);
	shader->setUniform("model", model);
	shader->setUniform("view", view);

	glBindTexture(GL_TEXTURE_2D, texture);
	shader->setUniform("ourTexture1", 0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GameObject::DrawElement(glm::mat4 projection, glm::mat4 view)
{
	shader->use();
	shader->setUniform("projection", projection);
	shader->setUniform("model", model);
	shader->setUniform("view", view);

	glBindTexture(GL_TEXTURE_2D, texture);
	shader->setUniform("ourTexture1", 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Fcount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}