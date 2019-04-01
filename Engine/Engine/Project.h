#pragma once

#include <windows.h>
#include <Commdlg.h>
#include <Commctrl.h>
#pragma comment(lib,"Comctl32.Lib")

namespace Project
{
	void New(OPENFILENAME &ofn);
	void Save(OPENFILENAME &ofn);
	void Load(OPENFILENAME &ofn);
}
