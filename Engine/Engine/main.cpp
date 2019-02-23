#include <shlobj.h>
#include "WinApi.h"

void EnableOpenGL();
void DisableOpenGL();


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
{
	AllocConsole();
	WinApi::debugConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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

	EnableOpenGL();

	WinApi::isLoaded = true;

	WinApi::Loop();

	DisableOpenGL();

	return 0;
}

void EnableOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;

	int iFormat;

	WinApi::hDC = GetDC(WinApi::hWndRender);

	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(WinApi::hDC, &pfd);
	SetPixelFormat(WinApi::hDC, iFormat, &pfd);
	WinApi::hRC = wglCreateContext(WinApi::hDC);
	wglMakeCurrent(WinApi::hDC, WinApi::hRC);
}

void DisableOpenGL()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(WinApi::hRC);
	ReleaseDC(WinApi::hWndRender, WinApi::hDC);
}