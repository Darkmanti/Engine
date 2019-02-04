#pragma once

#include <windows.h>
#include <stdint.h>

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

namespace Input
{
	extern uint32_t sizeButton;

	extern HWND hBtn_Shader;
	extern HWND hBtn_OpenModel;
	extern HWND hBtn_ShowModel;
	extern HWND hBtn_CloseModel;

	extern HMENU hMenu;
	extern HMENU hPopMenuFile;
	extern HMENU hPopMenuProject;
	extern HMENU hPopMenuProjectImport;

	extern HWND hStatusWindow;
};
