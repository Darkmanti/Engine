#pragma once

#include <algorithm>

#include "Shader.h"

class Mesh;

namespace Importer
{
	uint8_t ImportObj(const char* objPath, const char* dirPath, Mesh* Meshs);
};