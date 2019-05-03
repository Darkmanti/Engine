#include <shlobj.h>
#include <stdint.h>
#include <string>

#include "Debug.h"
#include "Config.h"
#include "Interface.h"
#include "Shader.h"

void EnableOpenGL();
void DisableOpenGL();

//#define _DEBUG_

#ifndef _DEBUG_

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
{
	debugConsole = nullptr;

	{
		std::stringstream params(lpCmdLine);
		std::string cmd;

		do
		{
			params >> cmd;

			if (cmd == "console")
			{
				AllocConsole();
				debugConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			}
		}
		while (!params.eof());
	}

	LoadConfigInterface();

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
	SaveConfigInterface();

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

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>

#include <ctime>

//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
int main()
{
	// денис в памяти занимает 15 мб
	clock_t t0 = clock(), t1;

	std::cout << "Timer init\n";

	std::fstream file("C:\\Users\\admin\\source\\repos\\Engine\\Engine\\Engine\\Resource\\denis\\denis.obj");
	std::string str;

	std::cout << "File open\n";

	char buf[8192];
	file.rdbuf()->pubsetbuf(buf, sizeof(buf));

	// Загружается за 10.296с 11.281 10.418 :::::::::::::::::

	file.rdbuf();
	// ::::::::::::::::::

	std::cout << "File data is copy in memory\n";

	file.close();

	t1 = clock();
	std::cout << "\nFile close: " << (double)(t1 - t0) / CLOCKS_PER_SEC << std::endl;

	std::cout << str;

	t1 = clock();
	std::cout << "\nOut file: " << (double)(t1 - t0) / CLOCKS_PER_SEC << std::endl;

	while (1)
	{

	}

	//InitWindow(&hInstance);
	//EnableOpenGL();
	//
	//Loop();

	//Shader *ourShader = new Shader("Shader//shader.vs", "Shader//shader.fs");
}

#endif