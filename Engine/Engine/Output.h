#pragma once

#include "GLEW/glew.h"

#include <windows.h>
#include <stdint.h>

// Все что связанно с окном и выводом winapi
namespace Output
{
	extern GLuint			windowWidth;			// Ширина окна редактора
	extern GLuint			windowHeight;			// Высота окна редактора

	extern GLuint			windowRenderWidth;		// Ширина окна рендера. Изменяется относительно размера
	extern GLuint			windowRenderHeight;		// Высота окна рендера. Изменяется относительно размера


	extern WNDCLASSEX		pWndClassEx;			// Структура класса окна


	extern GLuint			vArrayObject;			// VAO
	extern GLuint			shaderProgram;			// Шейдерная программа


	// Различные дескрипторы
	extern HDC			hDC;						// Дескриптор устройства
	extern HGLRC		hRC;						// Дескпритор ...

	extern HWND hWnd;								// Главное окно редактора
	extern HWND hWindowRender;						// Окно рендера внутри редактора

	// Регистрация окна
	ATOM RegisterEngineWindow(const HINSTANCE hInstance);

	// Создание окна
	uint8_t CreateEngineWindow(const int32_t nCmdShow);

	// Функция обработчика сообщений (Процедура окна)
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Метод с циклом программы
	void Loop();

	// Инициализация интерфейса
	int16_t InitInterface();

	void EnableOpenGL();
	void DisableOpenGL();
};
