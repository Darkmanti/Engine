#define GLEW_STATIC

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window;
	if (!(window = glfwCreateWindow(800, 600, "Engine", nullptr, nullptr)))
	{
		MessageBox(NULL, "GLFW не смог создать окно", "Ошибка", MB_ICONERROR | MB_OK);
		glfwTerminate();

		return 1;
	}

	// Подключем колбэки
	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowCloseCallback(window, closeCallback);

	// Создали контекст окна, который будет основным контекстом в данном потоке
	glfwMakeContextCurrent(window);

	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}

	glClearColor(.4f, .3f, .2f, 1.f);

	GLfloat vertexes[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
	};

	// Id объекта verticie buffer
	GLuint VBO;

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

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		MessageBox(NULL, "Вертексный шейдер не загружен", "Ошибка", MB_ICONERROR | MB_OK);
	}

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
