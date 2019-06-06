#include "GameObject.h"

#include "Importer.h"
#include "Shader.h"

#include "Mesh.h"
#include "Vector.h"

#include <cstring>
#include <fstream>

GameObject::GameObject(Shader* _shader, const char* dirPath)
{
	// Заполняем поле path
	path = dirPath;

	// Инициализируем массив с названиями скриптов в файле
	scriptNames = new char*[16];

	for (int i(0); i < 16; ++i)
	{
		scriptNames[i] = new char[256];
	}

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

		ImportObj(objPath, dirPath, Meshs);

		path = dirName;

		Debug("Loading object "); Debug(dirName); Debug(" succes\n");
	}
	else 
	{
		Debug("Error loading object "); Debug(dirName); Debug(". Invalid path "); Debug(dirPath); Debug("\n");
	}

	is_Select = false;

	angle_x = 0;
	angle_y = 0;
	angle_z = 0;
}

Vector &GameObject::get_location()
{
	return location;
}

Vector &GameObject::get_rotation()
{
	return rotation;
}

Vector &GameObject::get_scale()
{
	return scale;
}

void GameObject::set_location()
{
	p_location->x = location.x;
	p_location->y = location.y;
	p_location->z = location.z;

	// Добавить rotation
	setModel(glm::vec3(scale.x, scale.y, scale.z), glm::vec3(location.x, location.y, location.z), 0.0f, glm::vec3(0, 0, 0));
}

void GameObject::set_rotation()
{
	p_rotation->x = rotation.x;
	p_rotation->y = rotation.y;
	p_rotation->z = rotation.z;

	// Добавить rotation
	setModel(glm::vec3(scale.x, scale.y, scale.z), glm::vec3(location.x, location.y, location.z), 0.0f, glm::vec3(0, 0, 0));
}

void GameObject::set_scale()
{
	p_scale->x = scale.x;
	p_scale->y = scale.y;
	p_scale->z = scale.z;

	// Добавить rotation
	setModel(glm::vec3(scale.x, scale.y, scale.z), glm::vec3(location.x, location.y, location.z), 0.0f, glm::vec3(0, 0, 0));
}

void GameObject::setModel(glm::vec3 trans_, glm::vec3 model_, GLfloat degree_, glm::vec3 axis_)
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, model_);
	model = glm::rotate(model, degree_, axis_);
	model = glm::scale(model, trans_);
}

void GameObject::rotateModel(float angle_, glm::vec3 axis)
{
	model = glm::rotate(model, angle_, axis);
}

void GameObject::Draw(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos)
{
	shader->use();
	
	//:::::::::::::::::::::::: В Р Е М Е Н Н О :::::::::::::::::::::::::::::::
	shader->setUniform("material.diffuse", 0);

	shader->setUniform("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader->setUniform("dirLight.ambient", 0.1f, 0.1f, 0.1f);
	shader->setUniform("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	shader->setUniform("dirLight.specular", 0.5f, 0.5f, 0.5f);
	//:::::::::::::::::::::::: В Р Е М Е Н Н О :::::::::::::::::::::::::::::::

	shader->setUniform("model", model);
	shader->setUniform("projection", projection);
	shader->setUniform("view", view);
	shader->setUniform("viewPos", viewPos);

	for (int i = 0; i < obj_count; i++)
	{
		Meshs[i].DrawMesh();
	}
}

GameObject::~GameObject()
{
	for (int i(0); i < 16; ++i)
	{
		delete[] scriptNames[i];
	}

	delete[] scriptNames;

	for (int i = 0; i < obj_count; i++)
	{
		Meshs[i].~Mesh();
	}
}
