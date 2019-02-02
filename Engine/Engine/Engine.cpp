#include "Engine.h"

#include <fstream>
#include <map>

#include "Output.h"
#include "Input.h"

// Нужно решить проблему с разными значениями. ЗНАЧЕНИЯ ДОЛЖНЫ БЫТЬ ОДИНАКОВЫМИ
enum Settings
{
	WINDOWWIDTH			= 0,
	WINDOWHEIGHT		= 1,
	WINDOWRENDERWIDTH	= 2,
	WINDOWRENDERHEIGHT	= 3,
	SIZEBUTTON			= 4,
};

#define DEFAULT_WINDOWWIDTH			640
#define DEFAULT_WINDOWHEIGHT		480
#define DEFAULT_WINDOWRENDERWIDTH	640
#define DEFAULT_WINDOWRENDERHEIGHT	468
#define DEFAULT_SIZEBUTTON			12

static int LoadDefaultSettings()
{
	Output::windowWidth			= DEFAULT_WINDOWWIDTH;
	Output::windowHeight		= DEFAULT_WINDOWHEIGHT;
	Output::windowRenderWidth	= DEFAULT_WINDOWRENDERWIDTH;
	Output::windowRenderHeight	= DEFAULT_WINDOWRENDERHEIGHT;
	Input::sizeButton			= DEFAULT_SIZEBUTTON;

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
		}

		std::string str;
		char Cstr[256];

		// Если чтение вызывает исключение
		try
		{
			std::map <std::string, int> setting;

			setting["windowWidth"]			= WINDOWWIDTH;
			setting["windowHeight"]			= WINDOWHEIGHT;
			setting["windowRenderWidth"]	= WINDOWRENDERWIDTH;
			setting["windowRenderHeight"]	= WINDOWRENDERHEIGHT;
			setting["sizeButton"]			= SIZEBUTTON;

			while (!file.eof())
			{
				file.getline(Cstr, 255);
				str = Cstr;

				if (str[0] == '/' || str[0] == '\0')
				{
					continue;
				}

				std::string param;
				uint32_t valueU(0);
				int32_t valueI(0);
				double_t valueD(0);

				// Парсим строку
				{
					// Флаг нахождения значения
					bool isValue(false);
					std::string strValue;

					for (int i = 0, j = 0; i < str.length(); ++i, ++j)
					{
						if (str[i] >= 'a' && str[i] <= 'z' || str[i] >= 'A' && str[i] <= 'Z')
						{
							if (isValue)
							{
								if (str[i] == 'u')
								{
									valueU = std::stoul(strValue);
									break;
								}
								else if (str[i] == 'i')
								{
									valueI = std::stoi(strValue);
									break;
								}
								else if (str[i] == 'd')
								{
									valueD = std::stod(strValue);
									break;
								}
							}

							param += str[i];
						}
						else if (str[i] >= '0' && str[i] <= '9')
						{
							if (!isValue)
							{
								param += str[i];
							}
							else
							{
								strValue += str[i];
								--j;
							}
						}
						else if (str[i] == '=')
						{
							isValue = true;
						}
					}
				}

				// И записываем данные
				switch (setting[param])
				{
					case WINDOWWIDTH:
						Output::windowWidth = valueU;
						break;
					case WINDOWHEIGHT:
						Output::windowHeight = valueU;
						break;
					case WINDOWRENDERWIDTH:
						Output::windowRenderWidth = valueU;
						break;
					case WINDOWRENDERHEIGHT:
						Output::windowRenderHeight = valueU;
						break;
					case SIZEBUTTON:
						Input::sizeButton = valueU;
						break;
				default:
					break;
				}
			}
		}
		catch (int) // Если любое исключение, то
		{
			MessageBox(Output::hWnd, "Сбой файла конфигурации. Сброс к стандартным настройкам", "Ошибка", MB_OK);
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

		file << "// File config"												<< '\n';
		file << "windowWidth \t\t= " << Output::windowWidth << 'u'				<< '\n';
		file << "windowHeight \t\t= " << Output::windowHeight << 'u'			<< '\n';
		file << "windowRenderWidth \t= " << Output::windowRenderWidth << 'u'	<< '\n';
		file << "windowRenderHeight \t= " << Output::windowRenderHeight << 'u'	<< '\n';
		file << "sizeButton \t\t= " << Input::sizeButton << 'u'					<< '\n';

		file.close();
	}
};
