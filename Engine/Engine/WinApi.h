#pragma once

#include "GLEW/glew.h"

#include <windows.h>
#include <stdint.h>

#include "Shader.h"
#include "Engine.h"
#include "Graphics.h"
#include "Camera.h"
#include "GameObject.h"

// #define MENU_FILE				1000
#define MENU_FILE_NEWLOCATION		1001
#define MENU_FILE_OPENLOCATION		1002
#define MENU_FILE_SAVELOCATION		1003
#define MENU_FILE_SAVEASLOCATION	1004
// #define MENU_FILE_SEPARATOR		1005
#define MENU_FILE_QUIT				1006

// #define MENU_PROJECT				2000
#define MENU_PROJECT_IMPORT			20010
#define MENU_PROJECT_IMPORT_MODEL	20011
#define MENU_PROJECT_IMPORT_TEXTURE 20012
#define MENU_PROJECT_TEST			2002

#define VK_0						0x30
#define VK_1						0x31
#define VK_2						0x32
#define VK_3						0x33
#define VK_4						0x34
#define VK_5						0x35
#define VK_6						0x36
#define VK_7						0x37
#define VK_8						0x38
#define VK_9						0x39

#define VK_A						0x41
#define VK_B						0x42
#define VK_C						0x43
#define VK_D						0x44
#define VK_E						0x45
#define VK_F						0x46
#define VK_G						0x47
#define VK_H						0x48
#define VK_I						0x49
#define VK_J						0x4A
#define VK_K						0x4B
#define VK_L						0x4C
#define VK_M						0x4D
#define VK_N						0x4E
#define VK_O						0x4F
#define VK_P						0x50
#define VK_Q						0x51
#define VK_R						0x52
#define VK_S						0x53
#define VK_T						0x54
#define VK_U						0x55
#define VK_V						0x56
#define VK_W						0x57
#define VK_X						0x58
#define VK_Y						0x59
#define VK_Z						0x5A

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

						hWndSb,

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

	extern OPENFILENAME	OFN;

	extern uint8_t		setValue,
						*keys;

	// Регистрация окна
	ATOM RegisterWindowEngine();
	ATOM RegisterWindowRender();
	ATOM RegisterWindowLocation();
	ATOM RegisterWindowProject();

	uint8_t GetMask(WPARAM key);

	// Создание окна
	uint16_t CreateWindowEngine();
	uint16_t CreateWindowRender();
	uint16_t CreateWindowLocation();
	uint16_t CreateWindowProject();
	uint16_t CreateListViewLocation();
	uint16_t CreateListViewProject();
	uint16_t CreateButtons();
	uint16_t CreateMainMenu();

	uint16_t AddListViewLocation(const char* pCol1);

	// Инициализация интерфейса
	uint16_t InitInterface();
	uint16_t ShowInterface(const int16_t nCmdShow);

	// Функция обработчика сообщений (Процедура окна)
	LRESULT WndEngineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndRenderProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndLocationProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndProjectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool LocationAddItem(char* pCol1);

	// Метод с циклом программы
	void Loop(GameObject *models, const uint16_t countModels);
	void SetKeys(UINT message, WPARAM wParam, LPARAM lParam);
	bool GetKey(int8_t key);
};
