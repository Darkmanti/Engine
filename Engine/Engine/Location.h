#pragma once

#include <stdint.h>
#include <windows.h>
#include <Commdlg.h>
#include <Commctrl.h>
#pragma comment(lib,"Comctl32.Lib")

namespace Location
{
	extern int32_t gameobject_count;
	void New(OPENFILENAME &ofn);
	void Save(OPENFILENAME &ofn);
	void Load();
}
