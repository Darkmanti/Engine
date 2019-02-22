#pragma once

#include <windows.h>
#include <stdint.h>
#include <string>

// #define MENU_FILE				1000
#define MENU_FILE_NEWLOCATION		1001
#define MENU_FILE_OPENLOCATION		1002
#define MENU_FILE_SAVELOCATION		1003
#define MENU_FILE_SAVEASLOCATION	1004
// #define MENU_FILE_SEPARATOR		1005
#define MENU_FILE_QUIT				1006

// #define MENU_SCENE				2000
#define MENU_SCENE_CREATE			2001
#define MENU_SCENE_DELETE			2002

// #define MENU_PROJECT				3000
#define MENU_PROJECT_IMPORT			30010
#define MENU_PROJECT_IMPORT_MODEL	30011
#define MENU_PROJECT_IMPORT_TEXTURE 30012
#define MENU_PROJECT_TEST			3002

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

typedef struct tagAPPLINFO
{
	char szAppName[40];
	char szIconName[20];
	UINT iCost;
}APPLINFO;

// Все что связанно с окном и выводом winapi
namespace WinApi
{
	// Различные дескрипторы
	extern HWND			hWndEngine,							// Главное окно редактора
		hWndRender,							// Окно рендера внутри редактора

		hWndListViewLocation,				// ListView локация
		hWndListViewProject;				// ListView проект

	extern bool			isLoaded;

	extern HINSTANCE	hInstance;

	// Регистрация окна
	ATOM RegisterWindowEngine();
	ATOM RegisterWindowRender();

	// Создание окна
	uint16_t CreateWindowEngine();
	uint16_t CreateWindowRender();
	uint16_t CreateListViewLocation();
	uint16_t CreateListViewProject();
	uint16_t CreateMainMenu();

	// Инициализация интерфейса
	uint16_t InitInterface();
	uint16_t ShowInterface(const int16_t nCmdShow);

	// Функция обработчика сообщений (Процедура окна)
	LRESULT WndEngineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndRenderProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK SubClassLocationProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	LRESULT CALLBACK SubClassProjectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

	bool ListViewAddItem(const char* elementName, HWND hWndListView);

	// Метод с циклом программы
	void Loop();
	void InitInput();

	bool isKeyDown(int key);
	bool isKeyFirstPressed(int key);
	bool isKeyFirstReleased(int key);
};
