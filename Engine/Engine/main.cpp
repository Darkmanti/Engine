#define GLEW_STATIC

#include "GLEW/glew.h"

#include <shlobj.h>

#include "Graphics.h"
#include "Engine.h"
#include "WinApi.h"
#include "Shader.h"
#include "GameObject.h"
#include "Importer.h"

int WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
{
	Engine::isLoaded = false;

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

	// Создаем интерфейс (кнопки, хуепки, inputText и пр.)
	if (int16_t iError = WinApi::InitInterface()) // Проверка на создание интерфейса
	{
		std::string out("Ошибка ");

		out += std::to_string(iError);
		
		MessageBox(WinApi::hWndRender, "Интерфейс не создан", out.c_str(), MB_OK);

		return 0;
	}

	// Показываем интерфейс
	if (int16_t iError = WinApi::ShowInterface(nCmdShow))
	{
		std::string out("Ошибка ");

		out += std::to_string(iError);

		MessageBox(WinApi::hWndRender, "Интерфейс не отобразился", out.c_str(), MB_OK);

		return 0;
	}

	// Инициализация OpenGL
	Graphics::EnableOpenGL();

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
	if (int16_t iError = glewInit())
	{
		char out[64];

		//sprintf(out, "Ошибка %d", iError);

		MessageBox(NULL, "Glew не инициализироан", out, MB_ICONERROR | MB_OK);
		//glfwTerminate();
		return 1;
	}

	Engine::isLoaded = true;

	GameObject models[1];
	Importer::ImportObj("iron.obj", models[0]);

	for (uint16_t i = 0; i < 1; ++i)
	{
		models[i].Start();
	}

	WinApi::Loop(models, 1);

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

	Graphics::DisableOpenGL();
	
	delete Engine::camera;

	Engine::SaveConfigSettingsInterface();

	return 0;
}
