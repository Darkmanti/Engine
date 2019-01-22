#include "buttons.h"

#include <windows.h>

HWND hBtn_OpenModel;
HWND hBtn_WindowRender;

HWND &getHBtnOpenModel()
{
	return hBtn_OpenModel;
}

HWND &getHBtnWindowRender()
{
	return hBtn_WindowRender;
}
