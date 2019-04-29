#include "Location.h"

#include <fstream>

#include "GameObject.h"
#include "Shader.h"

int32_t gameobject_count;
GameObject* object_list = (GameObject*)malloc(0);

void New(OPENFILENAME &ofn)
{
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWndEngine;
	ofn.lpstrFilter = "Файл локации DarkMantiEngine (*.dmel)\0*.dmel\0Все файлы (*.*)\0*.*\0\0";
	ofn.lpstrFile = (LPSTR)lastLocationFileName.c_str();
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = (LPCSTR)"dmel";
	ofn.lpstrTitle = "Создать локацию";

	GetSaveFileName(&ofn);
	Debug("Создаю локацию "); Debug(ofn.lpstrFile);

	{
		char *tmp = ofn.lpstrFile;

		lastProjectFileName = std::string(tmp);
	}

	std::ofstream file;

	file.open(ofn.lpstrFile);

	file << "";

	Debug("Локация создана");
	file.close();
}

void Save(OPENFILENAME &ofn)
{
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWndEngine;
	ofn.lpstrFilter = "Файл локации DarkMantiEngine (*.dmel)\0*.dmel\0Все файлы (*.*)\0*.*\0\0";
	ofn.lpstrFile = (LPSTR)lastLocationFileName.c_str();
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = (LPCSTR)"dmel";
	ofn.lpstrTitle = "Сохранить локацию";

	if (!GetSaveFileName(&ofn))
	{
		Debug("Не удалось создать локацию");
	}

	Debug("Сохраняю локацию "); Debug(ofn.lpstrFile);

	{
		char *tmp = ofn.lpstrFile;

		lastProjectFileName = std::string(tmp);
	}

	std::ofstream file;

	file.open(ofn.lpstrFile);

	for (int i(0); i < gameobject_count; ++i)
	{
		file << object_list[i].name << "\n";
	}

	Debug("Локация сохранена");
	file.close();
}

void Load()
{
	// инициализируем объект gameobject
}

int8_t AddGameObject()
{
	object_list = (GameObject*)realloc(object_list, sizeof(GameObject) * (gameobject_count + 1));
	GameObject temp(&ourShader, "models/barrels");
	temp.setModel(glm::vec3(30.0f, 30.0f, 30.0f), glm::vec3(-20.0f, -20.0f, 20.0f), 9.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	object_list[gameobject_count] = temp;
	++gameobject_count;

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
