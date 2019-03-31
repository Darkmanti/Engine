#pragma once

#include <windows.h>
#include <fstream>

#include "WinApi.h"
//#include "GameObject.h"

namespace Location
{
	void New(OPENFILENAME &ofn);
	void Save(OPENFILENAME &ofn);
	void Load();
}
