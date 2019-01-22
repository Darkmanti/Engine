#include "globals.h"
#include "interface.h"
#include "buttons.h"

#ifndef ENGINE_GLOBALS

#include "GLEW/glew.h"

#include <windows.h>

#endif

#include <fstream>
#include <string>

extern std::string dirAppData;

void LoadConfigSettingsInterface()
{
	GLuint	&windowWidth(getWindowWidth()),
			&windowHeight(getWindowHeight()),

			&windowRenderWidth(getWindowRenderWidth()),
			&windowRenderHeight(getWindowRenderHeight());

	std::fstream file;

	file.open(dirAppData + std::string("\\settings.cfg"), std::ios::in);

	if (!file.is_open())
	{
		file.open(dirAppData + std::string("\\settings.cfg"), std::ios::out);

		file << 1366;			file << '\n';
		file << 768;			file << '\n';
		file << 1366;			file << '\n';
		file << 768 - 16;		file << '\n';

		file.close();

		windowWidth = 1366;
		windowHeight = 768;
		windowRenderWidth = 1366;
		windowRenderHeight = 768 - 16;

		return;
	}

	std::string::size_type size;
	std::string str;

	file >> str;
	windowWidth = std::stoi(str, &size);

	file >> str;
	windowHeight = std::stoi(str, &size);

	file >> str;
	windowRenderWidth = std::stoi(str, &size);

	file >> str;
	windowRenderHeight = std::stoi(str, &size);

	file.close();
}

void SaveConfigSettingsInterface()
{
	std::ofstream file;

	file.open(dirAppData + std::string("\\settings.cfg"), std::ios::out);

	if (!file.is_open())
	{
		return;
	}

	GLuint	&windowWidth(getWindowWidth()),
			&windowHeight(getWindowHeight()),

			&windowRenderWidth(getWindowRenderWidth()),
			&windowRenderHeight(getWindowRenderHeight());

	file << windowWidth;			file << '\n';
	file << windowHeight;			file << '\n';
	file << windowRenderWidth;		file << '\n';
	file << windowRenderHeight;		file << '\n';

	file.close();
}

int16_t InitInterface()
{
	// ViewportRender
	{
		HWND &hBtn(getHBtnWindowRender());
		hBtn = CreateWindow("BUTTON", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			0, 16, getWindowRenderWidth(), getWindowRenderHeight(), getHWnd(), 0, getHInstance(), NULL);

		// Возврат номера интерфейса который не создан
		if (!hBtn)
		{
			return 1;
		}

		ShowWindow(hBtn, SW_NORMAL);
	}

	// Кнопка загрузки модели
	{
		HWND &hBtn(getHBtnOpenModel());
		hBtn = CreateWindow("BUTTON", "Открыть модель",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			0, 0, 16, 16, getHWnd(), 0, getHInstance(), NULL);

		if (!hBtn)
		{
			return 2;
		}
		
		ShowWindow(hBtn, SW_NORMAL);
	}

	{

	}

	return 0;
}
