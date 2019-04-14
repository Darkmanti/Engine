#pragma once

#include <algorithm>

#include "Shader.h"

class Mesh;

namespace Importer
{
	uint32_t ImportObj(const char *fileName, GLuint& VAO, GLuint& VBO, GLuint& EBO, uint64_t& countV, uint64_t& countF);

	uint32_t Import(const char* objPath, const char* dirPath, Mesh* Meshs, int obj_count);
};