#pragma once

class Shader;

#include <STB/stb_truetype.h>

#include "GLEW/glew.h"

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include "Shader.h"

class FontObject
{
private:
	int width, height;
public:

	GLuint font;

	stbtt_bakedchar* cdata; // ASCII 32..126 is 95 glyphs

	Shader* shader;

	int firstChar;

	int glyphs;

	FontObject(	Shader*			shader_,	// Указатель на шейдер 
				int				height,		// Размер шрифта (высота)
				int				glyphs,		// Количество загружаемых символов (126 по умолчанию)
				char const*		filename,	// Путь к файлу шрифта
				int				firstChar_, // С какого символа начинать загрузку (32 по умолчанию)
				int x, int y);				// Размер изображения (bitmap) на которое должен уместиться весь шрифт

	void Print(float x, float y, const char* text, glm::vec3 color, glm::mat4 ortho);
};