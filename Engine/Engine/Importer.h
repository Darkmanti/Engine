#pragma once

#include <stdint.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sstream>

#include "Shader.h"
#include "GameObject.h"
#include <GLEW/glew.h>

namespace Importer
{
	uint32_t ImportObj(const char *fileName, GameObject* go, Shader* shader, GLuint texture);
};