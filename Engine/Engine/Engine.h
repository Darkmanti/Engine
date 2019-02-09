#pragma once

#include "GLEW/glew.h"

#include <windows.h>
#include <stdint.h>
#include <string>

// Настройки движка, winapi и прочее такое
namespace Engine
{
	extern HINSTANCE hInstance;			// Дескриптор приложения

	extern std::string dirAppData;		// Дирректория данных приложения

	// Загрузка конфига
	void LoadConfigSettingsInterface();

	// Сохранение конфига
	void SaveConfigSettingsInterface();
};
