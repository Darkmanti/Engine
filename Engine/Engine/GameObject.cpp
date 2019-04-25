#include "GameObject.h"

#include "Importer.h"
#include "Shader.h"

#include "Mesh.h"

#include <cstring>
#include <fstream>

GameObject::GameObject(Shader* _shader, const char* dirPath)
{
	shader = _shader;

	const char* dirName = strrchr(dirPath, 47) + 1;
	char objPath[512];
	strcpy(objPath, dirPath);
	strcat(objPath, "/");
	strcat(objPath, dirName);
	strcat(objPath, ".obj");

	obj_count = 0;

	std::ifstream file;
	file.open(objPath, std::ios_base::in);
	if (file.is_open())
	{
		char str[512];
		while (!file.eof())
		{
			file >> str;
			if (strlen(str) == 1)
			{
				if (strcmp(str, "o") == 0)
				{
					++obj_count;
				}
				else
				{
					file.getline(str, 512);
				}
			}
			else
			{
				file.getline(str, 512);
			}
		}
		file.close();

		Meshs = new Mesh[obj_count];

		Importer::Import(objPath, dirPath, Meshs);

		strcpy(name, dirName);

		WinApi::Debug("loading object :: "); WinApi::Debug(dirName); WinApi::Debug(" :: succes\n");
	}
	else 
	{
		WinApi::Debug("ERROR loading object :: "); WinApi::Debug(dirName); WinApi::Debug(" :: invalid path :: "); WinApi::Debug(dirPath); WinApi::Debug("\n");
	}

	is_Select = false;
}

GameObject::GameObject(Shader* shader_, Shader* selectShader_, const char* fileName, GLuint texture_)
{
	shader = shader_;
	selectShader = selectShader_;
	texture = texture_;

	Vcount = 0; Fcount = 0;
	Importer::ImportObj(fileName, VAO, VBO, EBO, Vcount, Fcount);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	
	is_Select = false;
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
	glBufferData(GL_ARRAY_BUFFER, (Vcount) * sizeof(GLfloat), vertices_, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
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

void GameObject::DrawArray(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos)
{
	shader->use();
	shader->setUniform("projection", projection);
	shader->setUniform("model", model);
	shader->setUniform("view", view);

	glBindTexture(GL_TEXTURE_2D, texture);
	shader->setUniform("material.diffuse", 0);

	shader->setUniform("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader->setUniform("dirLight.ambient", 0.1f, 0.1f, 0.1f);
	shader->setUniform("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	shader->setUniform("dirLight.specular", 0.5f, 0.5f, 0.5f);

	shader->setUniform("viewPos", viewPos);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, Vcount / 8);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GameObject::DrawArray_temp(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (is_Select == true)
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
	}
	else 
	{ 
		glStencilMask(0x00);
	}

	shader->use();
	shader->setUniform("projection", projection);
	shader->setUniform("model", model);
	shader->setUniform("view", view);

	glBindTexture(GL_TEXTURE_2D, texture);
	shader->setUniform("material.diffuse", 0);

	shader->setUniform("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader->setUniform("dirLight.ambient", 0.2f, 0.2f, 0.2f);
	shader->setUniform("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
	shader->setUniform("dirLight.specular", 0.5f, 0.5f, 0.5f);

	shader->setUniform("viewPos", viewPos);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, Fcount);

	if (is_Select == true)
	{
		glDisable(GL_DEPTH_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);

		selectShader->use();
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, texture);
		selectShader->setUniform("projection", projection);
		selectShader->setUniform("model", model);
		selectShader->setUniform("view", view);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::scale(trans, glm::vec3(1.1f, 1.1f, 1.1f));
		selectShader->setUniform("transform", trans);

		glDrawArrays(GL_TRIANGLES, 0, Fcount);

		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
}

void GameObject::Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos)
{
	shader->use();

	shader->setUniform("model", model);
	shader->setUniform("projection", projection);
	shader->setUniform("view", view);
	shader->setUniform("viewPos", viewPos);

	for (int i = 0; i < obj_count; i++)
	{
		Meshs[i].DrawMesh();
	}
}