#pragma once

#include <Windows.h>

#include "WinApi.h"

namespace Graphics
{
	void EnableOpenGL();

	void CreateObject(GLfloat vertices[], GLuint VAO, GLuint VBO);
	void DrawObject(GLuint VAO);
	void DisableOpenGL();
}
