#include "globals.h"

#include <string>

HINSTANCE		hInstance;				// Дескриптор приложения
WNDCLASSA		pWndClassA;				// Структура класса окна
HWND			hWnd;					// Дескриптор окна
HDC				hDC;					// Дескриптор устройства
HGLRC			hRC;					// Дескпритор ...

std::string		dirAppData;				// Путь расположения программы

GLuint			vArrayObject,			// VAO
				shaderProgram,			// Шейдерная программа передается в wndInit.cpp
	
				windowWidth,
				windowHeight,
	
				windowRenderWidth,		// Ширина вьюпорта сцены. Изменяется относительно размера
				windowRenderHeight;		// Высота вьюпорта сцены. Изменяется относительно размера

// Дескриптор приложения
HINSTANCE &getHInstance()
{
	return hInstance;
}

// Структура класса окна
WNDCLASSA	&getWndClassA()
{
	return pWndClassA;
}

// Дескриптор окна
HWND &getHWnd()
{
	return hWnd;
}

// Дескриптор устройства
HDC &getHDC()
{
	return hDC;
}

// Дескпритор ...
HGLRC &getHRC()
{
	return hRC;
}

// dirAppData
std::string &getDirAppData()
{
	return dirAppData;
}

// VAO
GLuint &getVArrayObject()
{
	return vArrayObject;
}

// Шейдерная программа
GLuint &getShaderProgram()
{
	return shaderProgram;
}

// Ширина окна
GLuint &getWindowWidth()
{
	return windowWidth;
}

// Высота окна
GLuint &getWindowHeight()
{
	return windowHeight;
}

// Ширина рендер-окна
GLuint &getWindowRenderWidth()
{
	return windowRenderWidth;
}

// Высота рендер-окна
GLuint &getWindowRenderHeight()
{
	return windowRenderHeight;
}