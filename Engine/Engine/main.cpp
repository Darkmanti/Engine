#include <shlobj.h>
#include "WinApi.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
{
	if (hPrevInstance)
	{
		MessageBox(NULL, "Можно запускать только одну копию приложения", "Ошибка", MB_OK | MB_ICONSTOP);
		return 1;
	}

	WinApi::isLoaded = false;

	// Отправляем дескриптор в файл wndInit
	WinApi::hInstance = hInstance;

	WinApi::InitInput();

	// Создаем интерфейс (кнопки, хуепки, inputText и пр.)
	if (int16_t iError = WinApi::InitInterface()) // Проверка на создание интерфейса
	{
		std::string out("Ошибка ");

		out += std::to_string(iError);

		MessageBox(WinApi::hWndRender, "Интерфейс не создан", out.c_str(), MB_OK);

		return 0;
	}

	// Показываем интерфейс
	if (int16_t iError = WinApi::ShowInterface(nCmdShow))
	{
		std::string out("Ошибка ");

		out += std::to_string(iError);

		MessageBox(WinApi::hWndRender, "Интерфейс не отобразился", out.c_str(), MB_OK);

		return 0;
	}

	WinApi::isLoaded = true;

	WinApi::Loop();

	return 0;
}
