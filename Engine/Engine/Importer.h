#pragma once

#include <stdint.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sstream>

// есть парочка (а может и больше) лишних инклудов
#include <stdio.h>
#include <string.h>
#include <cstring>

#include "Shader.h"
#include "GameObject.h"
#include <GLEW/glew.h>

namespace Importer
{
	uint32_t ImportObj(const char *fileName, GLuint& VAO, GLuint& VBO, GLuint& EBO, uint64_t& countV, uint64_t& countF);
};