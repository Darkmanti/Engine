#pragma once

#include <Windows.h>

#include "WinApi.h"

namespace Graphics
{
	extern GLfloat	lastFrame,
					deltaTime;

	void EnableOpenGL();
	void DisableOpenGL();
}
