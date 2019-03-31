#pragma once

#include <windows.h>
#include <stdint.h>
#include <string>

#include "Project.h"
#include "Location.h"

// #define MENU_SERVICE				1000
// #define MENU_SERVICE_SEPARATOR	1001
#define MENU_SERVICE_QUIT			1002

// #define MENU_LOCATION			2000
#define MENU_LOCATION_NEW			2001
#define MENU_LOCATION_OPEN			2002
#define MENU_LOCATION_SAVE			2003
#define MENU_LOCATION_ADDOBJECT		2004
#define MENU_LOCATION_DELETEOBJECT	2005

// #define MENU_PROJECT				3000
#define MENU_PROJECT_NEW			3001
#define MENU_PROJECT_OPEN			3002
#define MENU_PROJECT_SAVE			3003
#define MENU_PROJECT_IMPORT			30040
#define MENU_PROJECT_IMPORT_MODEL	30041
#define MENU_PROJECT_IMPORT_TEXTURE 30042
#define MENU_PROJECT_BUILD			3005
#define MENU_PROJECT_CLEAR			3006

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
	// Различные дескрипторы
	extern HWND					hWndEngine,								// Главное окно редактора
								hWndRender,												// Окно рендера внутри редактора

								hWndListViewLocation,									// ListView локация
								hWndListViewProject;									// ListView проект

	// Различные дескрипторы
	extern HDC					hDC;							// Дескриптор устройства
	extern HGLRC				hRC;							// Дескпритор ...

	extern HANDLE				debugConsole;

	extern bool					isLoaded;

	extern HINSTANCE			hInstance;

	extern std::string	lastProjectFileName,
						lastLocationFileName,
						projectPath;

	// Инициализация интерфейса
	uint16_t InitInterface();
	uint16_t ShowInterface(const int16_t nCmdShow);

	LRESULT WndEngineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndRenderProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK SubClassLocationProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	LRESULT CALLBACK SubClassProjectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

	bool ListViewAddItem(const char* elementName, HWND hWndListView);

	void InitInput();

	void Loop();

	bool isKeyDown(int key);
	void CameraControllAction();
	bool isKeyFirstPressed(int key);
	bool isKeyFirstReleased(int key);

	void Debug(const char* sms);
	void Clear();
};
