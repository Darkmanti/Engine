#pragma once

#ifndef ENGINE_GLOBALS

#include "GLEW/glew.h"

#include <windows.h>
#include <string>

#endif

HINSTANCE		&getHInstance();					// Дескриптор приложения
WNDCLASSA		&getWndClassA();					// Структура класса окна
HWND			&getHWnd();							// Дескриптор окна
HDC				&getHDC();							// Дескриптор устройства
HGLRC			&getHRC();							// Дескпритор ...
std::string		&getDirAppData();					// dirAppData
GLuint			&getVArrayObject();					// VAO
GLuint			&getShaderProgram();				// Шейдерная программа
GLuint			&getWindowWidth();					// Ширина окна
GLuint			&getWindowHeight();					// Высота окна
GLuint			&getWindowRenderWidth();			// Ширина рендер-сцены
GLuint			&getWindowRenderHeight();			// Высота рендер-сцены
