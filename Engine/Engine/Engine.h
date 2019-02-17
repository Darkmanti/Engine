#pragma once

#include "GLEW/glew.h"

#include <windows.h>
#include <stdint.h>
#include <string>

#include "Camera.h"

#define DEFAULT_WINDOW_POSITION_X				0
#define DEFAULT_WINDOW_POSITION_Y				0
#define DEFAULT_WINDOW_WIDTH					800
#define DEFAULT_WINDOW_HEIGHT					600

#define DEFAULT_WINDOW_RENDER_POSITION_X		128
#define DEFAULT_WINDOW_RENDER_POSITION_Y		16
#define DEFAULT_WINDOW_RENDER_WIDTH				672
#define DEFAULT_WINDOW_RENDER_HEIGHT			584

#define DEFUALT_WINDOW_LOCATION_POSITION_X		0
#define DEFUALT_WINDOW_LOCATION_POSITION_Y		24
#define DEFUALT_WINDOW_LOCATION_WIDTH			64
#define DEFUALT_WINDOW_LOCATION_HEIGHT			288

#define DEFUALT_WINDOW_PROJECT_POSITION_X		0
#define DEFUALT_WINDOW_PROJECT_POSITION_Y		312
#define DEFUALT_WINDOW_PROJECT_WIDTH			64
#define DEFUALT_WINDOW_PROJECT_HEIGHT			288

#define DEFAULT_IS_FULLSCREEN					0
#define DEFAULT_BUTTON_SIZE						24

// Настройки движка, winapi и прочее такое
namespace Engine
{
	extern HINSTANCE		hInstance;			// Дескриптор приложения

	extern std::string		dirAppData;		// Дирректория данных приложения

	extern Camera			*camera;

	extern bool				isLoaded;

	extern char				szDirect[MAX_PATH],
							szFileName[MAX_PATH];

	void NewLocation();
	void OpenLocation();
	void SaveLocation();
	void SaveAsLocation();

	// Загрузка конфига
	void LoadConfigSettingsInterface();

	// Сохранение конфига
	void SaveConfigSettingsInterface();
};
