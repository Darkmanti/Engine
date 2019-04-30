#include <shlobj.h>
#include <stdint.h>
#include <string>

#include "Interface.h"

#include "Shader.h";

void EnableOpenGL();
void DisableOpenGL();

//#define _DEBUG_

#ifndef _DEBUG_

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
{
	AllocConsole();
	debugConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hPrevInstance)
	{
		MessageBox(NULL, "Можно запускать только одну копию приложения", "Ошибка", MB_OK | MB_ICONSTOP);
		return 1;
	}	

	if (int8_t iError = InitWindow(&hInstance)) // Проверка на создание окна
	{
		std::string out("Ошибка ");

		out += std::to_string(iError);

		MessageBox(hWndEngine, "Интерфейс не создан", out.c_str(), MB_OK);

		return 0;
	}

	EnableOpenGL();
	InitVarInterface();

	InitImgui();

	isLoaded = true;

	Loop();

	DisableOpenGL();
	UninitVarInterface();

	return 0;
}

#endif

void EnableOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;

	int iFormat;

	hDC = GetDC(hWndEngine);

	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, iFormat, &pfd);
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
}

void DisableOpenGL()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWndEngine, hDC);
}

#ifdef _DEBUG_

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
{
	InitWindow(&hInstance);
	EnableOpenGL();
	
	Loop();

	Shader *ourShader = new Shader("Shader//shader.vs", "Shader//shader.fs");
}

#endif