#define GLEW_STATIC

#include "GLEW/glew.h"

#include <windows.h>
#include <stdint.h>
#include <shlobj.h>
#include <string>

#include "Engine.h"
#include "Output.h"
#include "Input.h"

int WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
{
	{
		// Нахождение пути данных программы
		char file[255];

		GetModuleFileName(NULL, file, 255);

		Engine::dirAppData = file;
		Engine::dirAppData = Engine::dirAppData.substr(0, Engine::dirAppData.find_last_of("\\"));
		Engine::dirAppData = Engine::dirAppData;
	}

	// загрузка программы
	Engine::LoadConfigSettingsInterface();

	// Отправляем дескриптор в файл wndInit
	Engine::hInstance = hInst;

	if (!Output::RegisterEngineWindow(hInst)) return 1; // Проверка на валидность регистрации окна
	if (!Output::CreateEngineWindow(nCmdShow)) return 1; // Проверка на валидность регистрации окна

	// Инициализация OpenGL
	Output::EnableOpenGL();

	//glfwInit();
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//GLFWwindow* window(glfwCreateWindow(800, 600, "Engine", nullptr, nullptr));
	//if (!window)
	//{
	//	MessageBox(NULL, "Окно не создано", "Ошибка", MB_ICONERROR | MB_OK);
	//	glfwTerminate();
	//	return 1;
	//}

	//glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit())
	{
		MessageBox(NULL, "Glew не инициализироан", "Ошибка", MB_ICONERROR | MB_OK);
		//glfwTerminate();
		return 1;
	}

	// Работа с боковой панелью
	//glViewport(256, 0, 800, 600);

	// Работа с верхней панелью
	glViewport(0, 0, 1366, 768 - 16);

	// Шейдерная программа
	Output::shaderProgram = glCreateProgram();

	// Шейдеры
	{
		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 position;\n"
			"void main()\n"
			"{\n"
			"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
			"}\0";
		glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShaderID);

		GLint iError;
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &iError);

		if (!iError)
		{
			GLchar infoLog[512];
			glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
			MessageBox(NULL, "Вертексный шейдер не скомпилировался" + *infoLog, "Ошибка", MB_ICONERROR | MB_OK);
		}

		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fragmentShaderSource = "#version 330 core\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\n\0";
		glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShaderID);

		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &iError);

		if (!iError)
		{
			GLchar infoLog[512];
			glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
			MessageBox(NULL, "Фрагментный шейдер не скомпилировался" + *infoLog, "Ошибка", MB_ICONERROR | MB_OK);
		}

		glAttachShader(Output::shaderProgram, vertexShaderID);
		glAttachShader(Output::shaderProgram, fragmentShaderID);
		glLinkProgram(Output::shaderProgram);


		glGetProgramiv(Output::shaderProgram, GL_COMPILE_STATUS, &iError);

		if (!iError)
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(Output::shaderProgram, 512, NULL, infoLog);
			MessageBox(NULL, "Шейдерная программа не собрана" + *infoLog, "Ошибка", MB_ICONERROR | MB_OK);
		}

		glUseProgram(Output::shaderProgram);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	Output::Loop();

	//while (!glfwWindowShouldClose(window))
	//{
	//	glfwPollEvents();

	//	glClearColor(0.4f, 0.4f, 0.8f, 1.0f);
	//	glClear(GL_COLOR_BUFFER_BIT);

	//	glUseProgram(shaderProgram);
	//	glBindVertexArray(getVArrayObject());
	//	glDrawArrays(GL_TRIANGLES, 0, 3);
	//	glBindVertexArray(0);

	//	glfwSwapBuffers(window);
	//}

	//glfwTerminate();

	Output::DisableOpenGL();

	Engine::SaveConfigSettingsInterface();
}
