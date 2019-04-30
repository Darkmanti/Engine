#include <fstream>
#include <vector>

#include "Interface.h"
#include "GameObject.h"
#include "Shader.h"

std::vector<GameObject*> object_list;

void InitVarLoc()
{
	
}

void UninitVarLoc()
{
	
}

void NewLoc()
{
	Debug("—оздаю локацию");

	Debug("—оздал");
}

void SaveLoc()
{
	Debug("—охран€ю локацию");

	Debug("—охранил");
}

void OpenLoc()
{
	Debug("ќткрываю локацию");

	Debug("ќткрыл");
}

int8_t AddGameObject(const char *path)
{
	static GameObject *temp;
	temp = new GameObject(ourShader, path);

	temp->setModel(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.f, -1.f, 1.f), 7.f, glm::vec3(0.f, 1.f, 0.f));
	object_list.push_back(temp);

	return 0;
}

int8_t AddScript()
{
	return 0;
}

int8_t RemoveGameObject()
{
	return 0;
}

int8_t RemoveScript()
{
	return 0;
}
