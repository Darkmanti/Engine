#define GLEW_STATIC

#include "GLEW\glew.h"
#include "GLFW\glfw3.h"

#include <windows.h>
#include <stdint.h>
#include <fstream>
#include <iostream>

// Предварительные объявления колбэков
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void closeCallback(GLFWwindow* window);

int main()
{
	glfwInit();

	// Устанавливает экспериментальный режим
	// Новые технологие и стабильная работа GLEW

	/*if (!glewInit())
	{
		MessageBox(NULL, "GLEW не смог инициализироваться", "Ошибка", MB_ICONERROR | MB_OK);
		return 1;
	}*/

	// Мин и макс версия OpenGL 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	
	//Установка профайла для которого создается контекст
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Manti Engine", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, 800, 600);

	/*GLFWwindow* window;
	if (!(window = glfwCreateWindow(800, 600, "Engine", nullptr, nullptr)))
	{
		MessageBox(NULL, "GLFW не смог создать окно", "Ошибка", MB_ICONERROR | MB_OK);
		glfwTerminate();

		return 1;
	}*/

	// Подключем колбэки
	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowCloseCallback(window, closeCallback);

	// Создали контекст окна, который будет основным контекстом в данном потоке

	int width = 800, height = 600;
	glfwGetFramebufferSize(window, &width, &height);

	GLfloat vertexes[] = {
		-0.5f, -0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
		-0.5f, -0.5f, 0.f,
	};

	// Id объекта verticie buffer
	GLuint VBO, VAO;

	// Выдача уникального id
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//MessageBox(NULL, static_cast<std::string>(), "Информация", MB_ICONINFORMATION | MB_OK);

	// Привязывает GL_ARRAY_BUFFER к нашему буферу
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Копируем все данные в буфер GL_STATIC_DRAW - содержание данных о вершине в не самой быстрой среде
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);

	while(!glfwWindowShouldClose(window))
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
