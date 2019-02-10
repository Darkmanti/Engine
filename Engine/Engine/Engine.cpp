#include "Engine.h"

#include <iostream>
#include <fstream>
#include <map>

#include "WinApi.h"

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

static int LoadDefaultSettings()
{
	WinApi::windowPositionX				= DEFAULT_WINDOW_POSITION_X;
	WinApi::windowPositionY				= DEFAULT_WINDOW_POSITION_Y;
	WinApi::windowWidth					= DEFAULT_WINDOW_WIDTH;
	WinApi::windowHeight				= DEFAULT_WINDOW_HEIGHT;

	WinApi::windowRenderPositionX		= DEFAULT_WINDOW_RENDER_POSITION_X;
	WinApi::windowRenderPositionY		= DEFAULT_WINDOW_RENDER_POSITION_Y;
	WinApi::windowRenderWidth			= DEFAULT_WINDOW_RENDER_WIDTH;
	WinApi::windowRenderHeight			= DEFAULT_WINDOW_RENDER_HEIGHT;

	WinApi::windowLocationPositionX		= DEFUALT_WINDOW_LOCATION_POSITION_X;
	WinApi::windowLocationPositionY		= DEFUALT_WINDOW_LOCATION_POSITION_Y;
	WinApi::windowLocationWidth			= DEFUALT_WINDOW_LOCATION_WIDTH;
	WinApi::windowLocationHeight		= DEFUALT_WINDOW_LOCATION_HEIGHT;

	WinApi::windowProjectPositionX		= DEFUALT_WINDOW_PROJECT_POSITION_X;
	WinApi::windowProjectPositionY		= DEFUALT_WINDOW_PROJECT_POSITION_Y;
	WinApi::windowProjectWidth			= DEFUALT_WINDOW_PROJECT_WIDTH;
	WinApi::windowProjectHeight			= DEFUALT_WINDOW_PROJECT_HEIGHT;

	WinApi::isFullscreen				= DEFAULT_IS_FULLSCREEN;
	WinApi::buttonsSize					= DEFAULT_BUTTON_SIZE;

	return 0;
}

namespace Engine
{
	HINSTANCE hInstance;		// Дескриптор приложения

	std::string dirAppData;		// Дирректория данных приложения

	// Загрузка конфига
	void LoadConfigSettingsInterface()
	{
		std::fstream file;

		file.open(dirAppData + std::string("\\settings.cfg"), std::ios::in);

		if (!file.is_open())
		{
			// Загрузка дефолтных настроек
			LoadDefaultSettings();

			return;
		}

		// Если чтение вызывает исключение
		try
		{
			std::string param;
			int32_t value;

			while (!file.eof())
			{
				file >> param;

				if (param == "//")
				{
					char cstr[64];
					file.getline(cstr, 64);
					continue;
				}

				if (param == "") continue;

				file >> value;

				// И записываем данные
				if		(param == "windowPositionX")				WinApi::windowPositionX = value;
				else if (param == "windowPositionY")				WinApi::windowPositionY = value;
				else if (param == "windowWidth")					WinApi::windowWidth = value;
				else if (param == "windowHeight")					WinApi::windowHeight = value;

				else if (param == "windowRenderPositionX")			WinApi::windowRenderPositionX = value;
				else if (param == "windowRenderPositionY")			WinApi::windowRenderPositionY = value;
				else if (param == "windowRenderWidth")				WinApi::windowRenderWidth = value;
				else if (param == "windowRenderHeight")				WinApi::windowRenderHeight = value;

				else if (param == "windowLocationPositionX")		WinApi::windowLocationPositionX = value;
				else if (param == "windowLocationPositionY")		WinApi::windowLocationPositionY = value;
				else if (param == "windowLocationWidth")			WinApi::windowLocationWidth = value;
				else if (param == "windowLocationHeight")			WinApi::windowLocationHeight = value;
				
				else if (param == "windowProjectPositionX")			WinApi::windowProjectPositionX = value;
				else if (param == "windowProjectPositionY")			WinApi::windowProjectPositionY = value;
				else if (param == "windowProjectWidth")				WinApi::windowProjectWidth = value;
				else if (param == "windowProjectHeight")			WinApi::windowProjectHeight = value;

				else if (param == "isFullscreen")					WinApi::isFullscreen = value;
				else if (param == "buttonsSize")					WinApi::buttonsSize = value;
			}
		}
		catch (int) // Если любое исключение, то
		{
			MessageBox(WinApi::hWndRender, "Сбой файла конфигурации. Сброс к стандартным настройкам", "Ошибка", MB_OK);
			LoadDefaultSettings();
		}

		file.close();
	}

	// Сохранение конфига
	void SaveConfigSettingsInterface()
	{
		std::ofstream file;

		file.open(dirAppData + std::string("\\settings.cfg"), std::ios::out);

		if (!file.is_open())
		{
			return;
		}

		// Компенсируем размер меню выравниванием значений
		WinApi::windowPositionX -= 8;
		WinApi::windowPositionY -= 31;
		WinApi::windowWidth += 16;
		WinApi::windowHeight += 39;

		WinApi::windowRenderPositionX -= 7;
		WinApi::windowRenderPositionY -= 7;
		WinApi::windowRenderWidth += 14;
		WinApi::windowRenderHeight += 39;

		WinApi::windowLocationPositionX -= 8;
		WinApi::windowLocationPositionY -= 31;
		WinApi::windowLocationWidth -= 116;
		WinApi::windowLocationHeight -= 19;

		WinApi::windowProjectPositionX -= 8;
		WinApi::windowProjectPositionY -= 31;
		WinApi::windowProjectWidth -= 116;
		WinApi::windowProjectHeight -= 19;

		file << "// File config" << '\n';

		file << "windowPositionX "				<< WinApi::windowPositionX << '\n';
		file << "windowPositionY "				<< WinApi::windowPositionY << '\n';
		file << "windowWidth "					<< WinApi::windowWidth << '\n';
		file << "windowHeight "					<< WinApi::windowHeight << '\n';

		file << "windowRenderPositionX "		<< WinApi::windowRenderPositionX << '\n';
		file << "windowRenderPositionY "		<< WinApi::windowRenderPositionY << '\n';
		file << "windowRenderWidth "			<< WinApi::windowRenderWidth << '\n';
		file << "windowRenderHeight "			<< WinApi::windowRenderHeight << '\n';

		file << "windowLocationPositionX "		<< WinApi::windowLocationPositionX << '\n';
		file << "windowLocationPositionY "		<< WinApi::windowLocationPositionY<< '\n';
		file << "windowLocationWidth "			<< WinApi::windowLocationWidth << '\n';
		file << "windowLocationHeight "			<< WinApi::windowLocationHeight << '\n';

		file << "windowProjectPositionX "		<< WinApi::windowProjectPositionX << '\n';
		file << "windowProjectPositionY "		<< WinApi::windowProjectPositionY << '\n';
		file << "windowProjectWidth "			<< WinApi::windowProjectWidth << '\n';
		file << "windowProjectHeight "			<< WinApi::windowProjectHeight << '\n';

		file << "isFullscreen "					<< WinApi::isFullscreen << '\n';
		file << "buttonsSize "					<< WinApi::buttonsSize << '\n';

		file.close();
	}
};
