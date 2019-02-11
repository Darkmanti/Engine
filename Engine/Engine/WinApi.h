#pragma once

#include "GLEW/glew.h"

#include <windows.h>
#include <stdint.h>

#include "Shader.h"
#include "Engine.h"
#include "Graphics.h"
#include "Camera.h"

// #define MENU_FILE				1000
#define MENU_FILE_NEWLOCATION		1001
#define MENU_FILE_SAVELOCATION		1002
#define MENU_FILE_SAVEASLOCATION	1003
// #define MENU_FILE_SEPARATOR		1004
#define MENU_FILE_QUIT				1005

// #define MENU_PROJECT				2000
#define MENU_PROJECT_IMPORT			20010
#define MENU_PROJECT_IMPORT_MODEL	20011
#define MENU_PROJECT_IMPORT_TEXTURE 20012
#define MENU_PROJECT_TEST			2002

// Все что связанно с окном и выводом winapi
namespace WinApi
{
	extern int32_t		windowPositionX,					// Позиция окна по X
						windowPositionY,					// Позиция окна по Y
						windowWidth,						// Ширина окна редактора
						windowHeight,						// Высота окна редактора

						windowRenderPositionX,				// Позиция окна рендера по X
						windowRenderPositionY,				// Позиция окна рендера по Y
						windowRenderWidth,					// Ширина окна рендера. Изменяется относительно размера
						windowRenderHeight,					// Высота окна рендера. Изменяется относительно размера

						windowLocationPositionX,			// Позиция окна location по X
						windowLocationPositionY,			// Позиция окна location по Y
						windowLocationWidth,				// Ширина окна location
						windowLocationHeight,				// Высота окна location

						windowProjectPositionX,				// Позиция окна location по X
						windowProjectPositionY,				// Позиция окна location по Y
						windowProjectWidth,					// Ширина окна location
						windowProjectHeight,				// Высота окна location

						isFullscreen,						// Развернутое окно или нет
						buttonsSize;						// Размер кнопок на панели инструментов


	extern WNDCLASSEX	pWndEngineClassEx,					// Структура класса окна
						pWndRenderClassEx,					// Структура класса рендер окна
						pWndLocationClassEx,				// Структура класса окна location
						pWndProjectClassEX;					// Структура класса окна project

	extern GLuint		vArrayObject,						// VAO
						vBufferObject,						// VBO
						shaderProgram;						// Шейдерная программа


	// Различные дескрипторы
	extern HDC			hDC;								// Дескриптор устройства
	extern HGLRC		hRC;								// Дескпритор ...

	extern HWND			hWndEngine,							// Главное окно редактора
						hWndRender,							// Окно рендера внутри редактора
						hWndLocation,						// Окно локации
						hWndProject,						// Окно проекта

						hWndListViewLocation,				// ListView локация
						hWndListViewProject,				// ListView проект

						hStatusWindow,

						// Кнопки
						hBtn_Shader,
						hBtn_OpenModel,
						hBtn_ShowModel,
						hBtn_CloseModel;

						// Меню
	extern HMENU		hMenu,
						hPopMenuFile,
						hPopMenuProject,
						hPopMenuProjectImport;

	extern Shader *shader;

	extern Camera *camera;

	extern bool isEnabled;

	// Регистрация окна
	ATOM RegisterWindowEngine();
	ATOM RegisterWindowRender();
	ATOM RegisterWindowLocation();
	ATOM RegisterWindowProject();

	// Создание окна
	uint16_t CreateWindowEngine();
	uint16_t CreateWindowRender();
	uint16_t CreateWindowLocation();
	uint16_t CreateWindowProject();
	uint16_t CreateListViewLocation();
	uint16_t CreateListViewProject();
	uint16_t CreateButtons();
	uint16_t CreateMainMenu();

	// Инициализация интерфейса
	uint16_t InitInterface();
	uint16_t ShowInterface(const int16_t nCmdShow);

	// Функция обработчика сообщений (Процедура окна)
	LRESULT WndEngineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndRenderProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndLocationProc(HWND hWndEngine, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndProjectProc(HWND hWndEngine, UINT message, WPARAM wParam, LPARAM lParam);

	// Метод с циклом программы
	void Loop();
};
