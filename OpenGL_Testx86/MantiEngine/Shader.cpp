#include "Shader.h"

// Генерация шейдера через конструктор
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// 1. Извлечение исхожного кода по указанному пути
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// гарантирует, что объекты ifstream могут генерировать исключения:
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// Открытие файла
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// Чтение из файлового буфера в потоковую переменную
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// Закрытие файла
		vShaderFile.close();
		fShaderFile.close();
		// Конвертация потока в строку
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	// Вывод ошибки при прочтении файла
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	// 2. Компиляция шейдера
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	// Вершинный шейдер
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Вывод ошибки в коде шейдера
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Фрагментный шейдер
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// вывод ошибки в коде шейдера
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Генерация программы шейдера
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	// Вывод ошибки сборки программы шейдера
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Удаление шейдеров, поскольку они теперь связаны с нашей программой и больше не нужны
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}
// Использование текущего шейдера
void Shader::Use()
{
	glUseProgram(this->Program);
}
