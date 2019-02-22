#include "Shader.h";

unsigned int ID;

// Конструктор генерирует шейдеры и шейдерную программу
Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	// 1. Получаем исходный код вершины / фрагмента из filePath
	std::string vertexCode,
				fragmentCode,
				geometryCode;

	std::ifstream	vShaderFile,
					fShaderFile,
					gShaderFile;

	// Убедитесь, что объекты потока могут генерировать исключения:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// Открываем файл
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream	vShaderStream, 
							fShaderStream;

		// Читаем содержимое файла в потоки
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Закрываем файлы
		vShaderFile.close();
		fShaderFile.close();

		// Конвертируем потоки в string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		// Если путь к геометрическому шейдеру указан, то подгружаем его исходники
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Файл не прочитан" << std::endl;
	}

	const char	*vShaderCode = vertexCode.c_str(),
				*fShaderCode = fragmentCode.c_str();

	// 2. Компилируем шейдеры
	unsigned int	vertex,
					fragment;

	// Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// Если исходник геометрического шейдера загружен, то компилим и его
	unsigned int geometry;

	if (geometryPath != nullptr)
	{
		const char * gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}

	// Шедерная программа
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);

	if (geometryPath != nullptr)
	{
		glAttachShader(ID, geometry);
	}

	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	// Удаляем шейдеры, они больше не нужны, т.к. находятся в шейдерной программе
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	if (geometryPath != nullptr)
	{
		glDeleteShader(geometry);
	}
}

// Активация шедерной программы
void Shader::use()
{
	glUseProgram(ID);
}

// utility uniform functions
void Shader::setUniform(const std::string &name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setUniform(const std::string &name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, const glm::vec2 &value)
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setUniform(const std::string &name, float x, float y)
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setUniform(const std::string &name, const glm::vec3 &value)
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setUniform(const std::string &name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setUniform(const std::string &name, const glm::vec4 &value)
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setUniform(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setUniform(const std::string &name, const glm::mat2 &mat)
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniform(const std::string &name, const glm::mat3 &mat)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setUniform(const std::string &name, const glm::mat4 &mat)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// utility function for checking shader compilation/linking errors.
void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}