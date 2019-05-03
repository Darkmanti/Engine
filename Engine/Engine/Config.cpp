#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>

#include "Debug.h"

uint16_t	windowX,
			windowY,
			windowWidth,
			windowHeight;

bool		fullscreen;

// Сохранение интерфейса
void SaveConfigInterface()
{
	std::ofstream file("settings.cfg");

	if (!file.is_open())
	{
		Debug("Failed to write data to the configuration file\n");
	}
	else
	{
		file << "windowX ";			file << windowX;		file << '\n';
		file << "windowY ";			file << windowY;		file << '\n';
		file << "windowWidth ";		file << windowWidth;	file << '\n';
		file << "windowHeight ";	file << windowHeight;	file << '\n';
		file << "fullscreen ";		file << fullscreen;		file << '\n';

		file.close();
	}
}

void ResetConfig()
{
	windowX = 0;
	windowY = 0;

	windowWidth = 1366;
	windowHeight = 768;

	fullscreen = false;

	SaveConfigInterface();
}

// Загрузка интерфейса
void LoadConfigInterface()
{
	std::ifstream file("settings.cfg");

	if (!file.is_open())
	{
		Debug("Could not read data from configuration file\n");
		ResetConfig();
		return;
	}

	char	key[256] = "",
			value[256] = "";

	while (file)
	{
		file >> key;
		file >> value;

		if (!strcmp(key, "windowX"))
		{
			windowX = atof(value);
		}
		else if (!strcmp(key, "windowY"))
		{
			windowY = atof(value);
		}
		else if (!strcmp(key, "windowWidth"))
		{
			windowWidth = atof(value);
		}
		else if (!strcmp(key, "windowHeight"))
		{
			windowHeight = atof(value);
		}
		else if (!strcmp(key, "fullscreen"))
		{
			fullscreen = atof(value);
		}
	}

	file.close();
}
