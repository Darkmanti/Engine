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
		// ���������� ���� ������ ���������
		char file[255];

		GetModuleFileName(NULL, file, 255);

		Engine::dirAppData = file;
		Engine::dirAppData = Engine::dirAppData.substr(0, Engine::dirAppData.find_last_of("\\"));
		Engine::dirAppData = Engine::dirAppData;
	}

	// �������� ���������
	Engine::LoadConfigSettingsInterface();

	// ���������� ���������� � ���� wndInit
	Engine::hInstance = hInst;

	// ���������� ��������� ��������� (������, ������, inputText � ��.)
	if (int16_t iError = Output::InitInterface(nCmdShow)) // �������� �� ������ �������� ����������
	{
		MessageBox(Output::hWnd, "��������� �� ������. ��� ������ - " + iError, "������", MB_OK);

		return 0;
	}

	// ������������� OpenGL
	Output::EnableOpenGL();

	//glfwInit();
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//GLFWwindow* window(glfwCreateWindow(800, 600, "Engine", nullptr, nullptr));
	//if (!window)
	//{
	//	MessageBox(NULL, "���� �� �������", "������", MB_ICONERROR | MB_OK);
	//	glfwTerminate();
	//	return 1;
	//}

	//glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit())
	{
		MessageBox(NULL, "Glew �� ��������������", "������", MB_ICONERROR | MB_OK);
		//glfwTerminate();
		return 1;
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
