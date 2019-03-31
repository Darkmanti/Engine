#pragma once

#include <windows.h>
#include <fstream>

#include "WinApi.h"
//#include "GameObject.h"

namespace Project
{
	void New(OPENFILENAME &ofn);
	void Save(OPENFILENAME &ofn);
	void Load(OPENFILENAME &ofn);
}
