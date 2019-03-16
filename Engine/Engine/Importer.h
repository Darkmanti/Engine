#pragma once

#include <stdint.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sstream>

// ���� ������� (� ����� � ������) ������ ��������, �� ��� ���� ����� memcpy � memmove �������(����� ��������)
#include <stdio.h>
#include <string.h>
#include <cstring>

#include "Shader.h"
#include "GameObject.h"
#include <GLEW/glew.h>

namespace Importer
{
	uint32_t ImportObj(const char *fileName, GLfloat* &V_, uint64_t& countV, GLuint* &F_, uint64_t& countF);
};