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

// ��� ��� �������� � ����� � ������� winapi
namespace WinApi
{
	extern int32_t		windowPositionX,					// ������� ���� �� X
						windowPositionY,					// ������� ���� �� Y
						windowWidth,						// ������ ���� ���������
						windowHeight,						// ������ ���� ���������

						windowRenderPositionX,				// ������� ���� ������� �� X
						windowRenderPositionY,				// ������� ���� ������� �� Y
						windowRenderWidth,					// ������ ���� �������. ���������� ������������ �������
						windowRenderHeight,					// ������ ���� �������. ���������� ������������ �������

						windowLocationPositionX,			// ������� ���� location �� X
						windowLocationPositionY,			// ������� ���� location �� Y
						windowLocationWidth,				// ������ ���� location
						windowLocationHeight,				// ������ ���� location

						windowProjectPositionX,				// ������� ���� location �� X
						windowProjectPositionY,				// ������� ���� location �� Y
						windowProjectWidth,					// ������ ���� location
						windowProjectHeight,				// ������ ���� location

						isFullscreen,						// ����������� ���� ��� ���
						buttonsSize;						// ������ ������ �� ������ ������������


	extern WNDCLASSEX	pWndEngineClassEx,					// ��������� ������ ����
						pWndRenderClassEx,					// ��������� ������ ������ ����
						pWndLocationClassEx,				// ��������� ������ ���� location
						pWndProjectClassEX;					// ��������� ������ ���� project

	extern GLuint		vArrayObject,						// VAO
						vBufferObject,						// VBO
						shaderProgram;						// ��������� ���������


	// ��������� �����������
	extern HDC			hDC;								// ���������� ����������
	extern HGLRC		hRC;								// ���������� ...

	extern HWND			hWndEngine,							// ������� ���� ���������
						hWndRender,							// ���� ������� ������ ���������
						hWndLocation,						// ���� �������
						hWndProject,						// ���� �������

						hWndListViewLocation,				// ListView �������
						hWndListViewProject,				// ListView ������

						hStatusWindow,

						// ������
						hBtn_Shader,
						hBtn_OpenModel,
						hBtn_ShowModel,
						hBtn_CloseModel;

						// ����
	extern HMENU		hMenu,
						hPopMenuFile,
						hPopMenuProject,
						hPopMenuProjectImport;

	extern OPENFILENAME	ofn;

	extern uint8_t		setValue,
						*keys;

	extern char			szDirect[MAX_PATH],
						szFileName[MAX_PATH];


	// ����������� ����
	ATOM RegisterWindowEngine();
	ATOM RegisterWindowRender();
	ATOM RegisterWindowLocation();
	ATOM RegisterWindowProject();

	uint8_t getMask(WPARAM wParam);

	// �������� ����
	uint16_t CreateWindowEngine();
	uint16_t CreateWindowRender();
	uint16_t CreateWindowLocation();
	uint16_t CreateWindowProject();
	uint16_t CreateListViewLocation();
	uint16_t CreateListViewProject();
	uint16_t CreateButtons();
	uint16_t CreateMainMenu();

	uint16_t AddListViewLocation(const char* pCol1);

	// ������������� ����������
	uint16_t InitInterface();
	uint16_t ShowInterface(const int16_t nCmdShow);

	// ������� ����������� ��������� (��������� ����)
	LRESULT WndEngineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndRenderProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndLocationProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT WndProjectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool LocationAddItem(char* pCol1);

	// ����� � ������ ���������
	void Loop(GameObject *models, const uint16_t countModels);
	void BindKey(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
