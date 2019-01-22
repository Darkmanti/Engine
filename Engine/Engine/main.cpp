// GLFWwindow* window = glfwCreateWindow(800, 600, "Engine", nullptr, nullptr);
// НЕОБХОДИМО ИНИЦИАЛИЗИРОВАТЬ окно ПЕРЕД glewInit и ПОСЛЕ glfwWindowHint();

#define GLEW_STATIC

#include "GLEW/glew.h"
//#include <GL/glu.h>

#include "globals.h"
#include "interface.h"
#include "buttons.h"

#include <windows.h>
//#include <iostream>
//#include <fstream>
//#include <string>
#include <stdint.h>
#include <string>
#include <shlobj.h>

// Пошли предварительные объявления
extern HINSTANCE	hInstance;		// Дескриптор приложения
extern HDC			hDC;			// Дескриптор устройства
extern HGLRC		hRC;			// Дескпритор ...
extern GLuint		shaderProgram;	// Шейдерная программа передается в wndInit.cpp


// Объявили будущий метод для последующей передачи его указателя в структуру WNDCLASSA
LRESULT WndProc(HWND, uint32_t, WPARAM, LPARAM);

// Объявили метод регистрации окна для семантической группировки стейтментов
ATOM RegisterEngineWindow(const HINSTANCE);

// Объявили создание окна
uint8_t CreateEngineWindow(const int32_t);

// Основной цикл программы
void Loop();


int WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
{
	{
		// Нахождение пути данных программы
		std::string &dirAppData = getDirAppData();
		char file[255];

		GetModuleFileName(NULL, file, 255);

		dirAppData = static_cast<std::string>(file);
		dirAppData = dirAppData.substr(0, dirAppData.find_last_of("\\"));
	}

	// загрузка программы
	LoadConfigSettingsInterface();

	// Отправляем дескриптор в файл wndInit
	hInstance = hInst;

	if (!RegisterEngineWindow(hInst)) return 1; // Проверка на валидность регистрации окна
	if (!CreateEngineWindow(nCmdShow)) return 1; // Проверка на валидность регистрации окна

	// Инициализация OpenGL
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;

	hDC = GetDC(getHBtnWindowRender());
	GLuint iPixelFormat = ChoosePixelFormat(hDC, &pfd);

	if (iPixelFormat != 0)
	{
		PIXELFORMATDESCRIPTOR bestMatch_pfd;
		DescribePixelFormat(hDC, iPixelFormat, sizeof(pfd), &bestMatch_pfd);

		if (bestMatch_pfd.cDepthBits < pfd.cDepthBits)
		{
			return 1;
		}

		if (SetPixelFormat(hDC, iPixelFormat, &pfd) == FALSE)
		{
			DWORD dwErrorCode = GetLastError();
			return 1;
		}
	}
	else
	{
		DWORD dwErrorCode = GetLastError();
		return 1;
	}

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);



	
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
	//glViewport(0, 0, 1366, 768 - 64);

	// Шейдерная программа
	shaderProgram = glCreateProgram();

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

		glAttachShader(shaderProgram, vertexShaderID);
		glAttachShader(shaderProgram, fragmentShaderID);
		glLinkProgram(shaderProgram);


		glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &iError);

		if (!iError)
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			MessageBox(NULL, "Шейдерная программа не собрана" + *infoLog, "Ошибка", MB_ICONERROR | MB_OK);
		}

		glUseProgram(shaderProgram);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	Loop();

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
	
	SaveConfigSettingsInterface();

	return 0;
}
