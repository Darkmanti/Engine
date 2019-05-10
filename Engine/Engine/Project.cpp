#include <fstream>

#include "Debug.h"

std::string pathProj;
std::string nameProj;

void NewProj()
{
	Debug("Create project\n");

	pathProj = "C:\\Users\\admin\\Documents\\DarkMantiEngine Projects\\3d runner";

	// Создается папка игрового проекта
	// В папке создается файл %nameProj%.dmep
	// Создается папка scripts в игровом проекте
	// Кидаются все движковые includes
	// Кидается вся реализация кода движка в dll библиотек(е/ах)

	Debug("Created\n");
}

void SaveProj()
{
	Debug("Save project\n");

	Debug("Saved\n");
}

void OpenProj()
{
	Debug("Open project\n");

	pathProj = "C:\\Users\\admin\\Documents\\DarkMantiEngine Projects\\3d runner";

	Debug("Opened\n");
}
