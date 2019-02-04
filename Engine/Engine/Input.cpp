#include "Input.h"

#include <windows.h>
#include <stdint.h>

namespace Input
{
	uint32_t sizeButton;

	HWND hBtn_Shader;
	HWND hBtn_OpenModel;
	HWND hBtn_ShowModel;
	HWND hBtn_CloseModel;

	HMENU hMenu;
	HMENU hPopMenuFile;
	HMENU hPopMenuProject;
	HMENU hPopMenuProjectImport;

	HWND hStatusWindow;
};