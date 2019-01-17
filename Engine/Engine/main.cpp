/*#define GLEW_STATIC

#include "GLEW\glew.h"
#include "GLFW\glfw3.h"

#include <windows.h>
#include <stdint.h>
#include <fstream>
#include <iostream>

int isExist(const char path[128])
{
	std::ifstream file;

	file.open(path, std::ios::in);

	if (!file.is_open())
	{
		return 0;
	}

	file.close();

	return 1;
}

// Предварительные объявления колбэков
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void closeCallback(GLFWwindow* window);

int WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
{
	glfwInit();

	// Устанавливает экспериментальный режим
	// Новые технологие и стабильная работа GLEW
	glewExperimental = GL_TRUE;

	if (!glewInit())
	{
		MessageBox(NULL, "GLEW не смог инициализироваться", "Ошибка", MB_ICONERROR | MB_OK);
		return 1;
	}

	// Мин и макс версия OpenGL 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window;
	if (!(window = glfwCreateWindow(800, 600, "Engine", nullptr, nullptr)))
	{
		MessageBox(NULL, "GLFW не смог создать окно", "Ошибка", MB_ICONERROR | MB_OK);
		glfwTerminate();

		return 1;
	}

	GLfloat vertexes[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
	};

	// Id объекта verticie buffer
	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Выдача уникального id
	glGenBuffers(1, &VBO);

	//MessageBox(NULL, static_cast<std::string>(), "Информация", MB_ICONINFORMATION | MB_OK);

	// Привязывает GL_ARRAY_BUFFER к нашему буферу
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Копируем все данные в буфер GL_STATIC_DRAW - содержание данных о вершине в не самой быстрой среде
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
	
	// Храним id объекта - шейдера
	GLuint vertexShader(glCreateShader(GL_VERTEX_SHADER));

	if (!isExist("vertex.glsl"))
	{
		MessageBox(NULL, "Файл не найден", "Ошибка", MB_ICONERROR | MB_OK);
		return 1;
	}

	std::ifstream file;
	file.open("vertex.glsl", std::ios::in);

	if (!file.is_open())
	{
		MessageBox(NULL, "Файл не открывается", "Ошибка", MB_ICONERROR | MB_OK);
		return 1;
	}
	else
	{
		GLchar* vertexShaderSource{};

		while (!file.eof())
		{
			file >> vertexShaderSource;
		}

		file.close();

		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	}

	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		MessageBox(NULL, "Вертексный шейдер не загружен", "Ошибка", MB_ICONERROR | MB_OK);
	}

	// Подключем колбэки
	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowCloseCallback(window, closeCallback);

	// Создали контекст окна, который будет основным контекстом в данном потоке
	glfwMakeContextCurrent(window);

	glClearColor(.4f, .3f, .2f, 1.f);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// Определение колбэков
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch(key)
	{
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS)
			{
				if (MessageBox(NULL, "Вы точно хотите выйти?", "Внимание", MB_ICONWARNING | MB_YESNO) == IDYES)
				{
					glfwSetWindowShouldClose(window, GL_TRUE);
				}
			}

			break;
		case GLFW_KEY_F5:
			if (action == GLFW_PRESS)
			{
				glClearColor(1.f / (rand() % 64), 1.f / (rand() % 64), 1.f / (rand() % 64), 1.f);
			}

			break;
	}
}

void closeCallback(GLFWwindow* window)
{
	if (MessageBox(NULL, "Вы точно хотите выйти?", "Внимание", MB_ICONWARNING | MB_YESNO) == IDYES)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}
*/

// const char* S = R"(dfsdf)";

#define GLEW_STATIC

#include "GLEW\glew.h"
#include "GLFW\glfw3.h"

#include <fstream>

int isExist(const GLchar*);
GLboolean loadShader(const GLint, const GLchar*, GLchar*);

int width(800), height(600);

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Engine", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	
	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, 800, 600);
	
	glfwGetFramebufferSize(window, &width, &height);

	// Пошел вертексный шейдер
	GLfloat vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	GLuint vertexArraysID;
	glGenVertexArrays(1, &vertexArraysID);
	glBindVertexArray(vertexArraysID);

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	
	GLuint shaderProgram = glCreateProgram();

	{
		// Создание объекта вертексного шейдера
		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		GLchar* vShaderSource{};

		//loadShader(GL_VERTEX_SHADER, "vShader.glsl", vShaderSource);

		glShaderSource(vShader, 1, &vShaderSource, NULL);
		glCompileShader(vShader);

		// Создание объекта фрагментного шейдера
		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLchar* fShaderSource{}; // Есть вероятность что фрагментный шейдер записываеися В КОНЕЦ вертексного

		//loadShader(GL_FRAGMENT_SHADER, "fShader.glsl", fShaderSource);

		glShaderSource(fShader, 1, &fShaderSource, NULL);
		glCompileShader(fShader);

		// Создаем шейдерную программу
		glAttachShader(shaderProgram, vShader);
		glAttachShader(shaderProgram, fShader);
		glLinkProgram(shaderProgram);

		glUseProgram(shaderProgram);

		glDeleteShader(vShader);
		glDeleteShader(fShader);
	}

	// Сообщяем OpenGL как он должен интерпретировать вершинные данные
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

// Проверяет наличие файла
int isExist(const GLchar* filePath)
{
	std::ifstream file;

	file.open(filePath, std::ios::in);

	if (file.is_open())
	{
		file.close();

		return 1;
	}

	return 0;
}

// Загрузка шейдеров из файлов v(fileName).glsl и f(fileName).glsl
// type = GL_VERTEX_SHADER или GL_FRAGMENT_SHADER
GLboolean loadShader(const GLint type, const GLchar* fileName, GLchar* shaderSource)
{
	if (!isExist(fileName))
	{
		return 1;
	}

	if (type == GL_VERTEX_SHADER)
	{
		std::ifstream file;

		file.open(static_cast<std::string>(fileName) + ".glsl", std::ios::in);

		if (!file.is_open())
		{
			return 1;
		}

		while (!file.eof())
		{
			file >> shaderSource;
		}

		file.close();

		return 0;
	}
	else if (type == GL_FRAGMENT_SHADER)
	{
		std::ifstream file;

		file.open(static_cast<std::string>(fileName) + ".glsl", std::ios::in);

		if (!file.is_open())
		{
			return 1;
		}

		while (!file.eof())
		{
			file >> shaderSource;
		}

		file.close();

		return 0;
	}

	return 1;
}