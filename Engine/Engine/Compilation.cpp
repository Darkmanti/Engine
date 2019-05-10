#include "Interface.h"
#include "Location.h"
#include "Debug.h"
#include "Project.h"

#include <fstream>

int Compilation()
{
	// Создание папки build
	std::string str = "mkdir \"" + pathProj + "\\build\\\"";
	Debug(str.c_str()); Debug("\n");
	system(str.c_str()); Debug("\n");

	std::ofstream file;

	str = pathProj + "\\scripts\\main.cpp";
	file.open(str);

	if (!file.is_open())
	{
		Debug("Compilation is cancel\n");
		Debug("File main.cpp is not open\n");
	}

	// Копия шаблона в проект
	file << ""
"#include <windows.h>																					  \n"
"#include <stdint.h>																					  \n"
"																										  \n"
"#include \"include/glew/glew.h\"																		  \n"
"																										  \n"
"#include \"include/glm/glm.hpp\"																		  \n"
"#include \"include/glm/gtc/matrix_transform.hpp\"														  \n"
"#include \"include/glm/gtc/type_ptr.hpp\"																  \n"
"																										  \n"
"#include \"Camera.h\"																					  \n"
"#include \"GameObject.h\"																				  \n"
"																										  \n"
"#define VK_0						0x30																  \n"
"#define VK_1						0x31																  \n"
"#define VK_2						0x32																  \n"
"#define VK_3						0x33																  \n"
"#define VK_4						0x34																  \n"
"#define VK_5						0x35																  \n"
"#define VK_6						0x36																  \n"
"#define VK_7						0x37																  \n"
"#define VK_8						0x38																  \n"
"#define VK_9						0x39																  \n"
"																										  \n"
"#define VK_A						0x41																  \n"
"#define VK_B						0x42																  \n"
"#define VK_C						0x43																  \n"
"#define VK_D						0x44																  \n"
"#define VK_E						0x45																  \n"
"#define VK_F						0x46																  \n"
"#define VK_G						0x47																  \n"
"#define VK_H						0x48																  \n"
"#define VK_I						0x49																  \n"
"#define VK_J						0x4A																  \n"
"#define VK_K						0x4B																  \n"
"#define VK_L						0x4C																  \n"
"#define VK_M						0x4D																  \n"
"#define VK_N						0x4E																  \n"
"#define VK_O						0x4F																  \n"
"#define VK_P						0x50																  \n"
"#define VK_Q						0x51																  \n"
"#define VK_R						0x52																  \n"
"#define VK_S						0x53																  \n"
"#define VK_T						0x54																  \n"
"#define VK_U						0x55																  \n"
"#define VK_V						0x56																  \n"
"#define VK_W						0x57																  \n"
"#define VK_X						0x58																  \n"
"#define VK_Y						0x59																  \n"
"#define VK_Z						0x5A																  \n"
"																										  \n"
"Camera camera;																							  \n"
"																										  \n"
"int64_t mouseOffsetX,																					  \n"
"mouseOffsetY,																							  \n"
"lastMousePosX,																							  \n"
"lastMousePosY;																							  \n"
"																										  \n"
"POINT* mousePos;																						  \n"
"																										  \n"
"uint16_t numberOfKeys;																					  \n"
"																										  \n"
"bool* previousKeyboardState;																			  \n"
"																										  \n"
"LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)						  \n"
"{																										  \n"
"switch (uMsg)																							  \n"
"{																										  \n"
"case WM_DESTROY:																						  \n"
"PostQuitMessage(0);																					  \n"
"break;																									  \n"
"default:																								  \n"
"return DefWindowProc(hwnd, uMsg, wParam, lParam);														  \n"
"}																										  \n"
"																										  \n"
"return 0;																								  \n"
"}																										  \n"
"																										  \n"
"bool isKeyDown(int key)																				  \n"
"{																										  \n"
"return (GetAsyncKeyState(key) & (1 << 16));															  \n"
"}																										  \n"
"																										  \n"
"bool isKeyFirstPressed(int key)																		  \n"
"{																										  \n"
"bool previousState = previousKeyboardState[key];														  \n"
"																										  \n"
"previousKeyboardState[key] = isKeyDown(key);															  \n"
"																										  \n"
"return (previousKeyboardState[key] && !previousState);													  \n"
"}																										  \n"
"																										  \n"
"bool isKeyFirstReleased(int key)																		  \n"
"{																										  \n"
"bool previousState = previousKeyboardState[key];														  \n"
"																										  \n"
"previousKeyboardState[key] = isKeyDown(key);															  \n"
"																										  \n"
"return (!previousKeyboardState[key] && previousState);													  \n"
"}																										  \n"
"																										  \n"
"int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)				  \n"
"{																										  \n"
"if (hPrevInstance)																						  \n"
"{																										  \n"
"MessageBox(NULL, \"Можно запускать только одну копию приложения\", \"Ошибка\", MB_OK | MB_ICONSTOP);	  \n"
"return 1;																								  \n"
"}																										  \n"
"																										  \n"
"previousKeyboardState = new bool[numberOfKeys];														  \n"
"for (uint16_t keyNum = 0; keyNum < numberOfKeys; ++keyNum)												  \n"
"{																										  \n"
"previousKeyboardState[keyNum] = isKeyDown(keyNum);														  \n"
"}																										  \n"
"																										  \n"
"int16_t windowWidth = GetSystemMetrics(SM_CXSCREEN),													  \n"
"windowHeight = GetSystemMetrics(SM_CYSCREEN);															  \n"
"																										  \n"
"WNDCLASSEX pWndEngineClassEx{};																		  \n"
"																										  \n"
"pWndEngineClassEx.cbSize = sizeof(WNDCLASSEX);															  \n"
"pWndEngineClassEx.style = CS_VREDRAW | CS_HREDRAW;														  \n"
"pWndEngineClassEx.lpfnWndProc = WindowProc;															  \n"
"pWndEngineClassEx.hInstance = hInstance;																  \n"
"pWndEngineClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);												  \n"
"pWndEngineClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);											  \n"
"pWndEngineClassEx.lpszClassName = \"WndEngineClass\";													  \n"
"pWndEngineClassEx.hIcon = LoadIcon(hInstance, \"IDI_ENGINEICON\");										  \n"
"																										  \n"
"RegisterClassEx(&pWndEngineClassEx);																	  \n"
"																										  \n"
"HWND hWndEngine = CreateWindowEx(WS_EX_ACCEPTFILES,													  \n"
"pWndEngineClassEx.lpszClassName,																		  \n"
"\"Game\", WS_OVERLAPPEDWINDOW | WS_POPUP,																  \n"
"0,																										  \n"
"0,																										  \n"
"windowWidth,																							  \n"
"windowHeight,																							  \n"
"0,																										  \n"
"0,																										  \n"
"hInstance,																								  \n"
"0																										  \n"
");																										  \n"
"																										  \n"
"if (!hWndEngine)																						  \n"
"{																										  \n"
"MessageBox(NULL, \"Window is not create\", \"Error\", MB_OK);											  \n"
"return 1;																								  \n"
"}																										  \n"
"																										  \n"
"ShowWindow(hWndEngine, SW_MAXIMIZE);																	  \n"
"UpdateWindow(hWndEngine);																				  \n"
"																										  \n"
"PIXELFORMATDESCRIPTOR pfd;																				  \n"
"																										  \n"
"int iFormat;																							  \n"
"																										  \n"
"HDC hDC = GetDC(hWndEngine);																			  \n"
"																										  \n"
"ZeroMemory(&pfd, sizeof(pfd));																			  \n"
"																										  \n"
"pfd.nSize = sizeof(pfd);																				  \n"
"pfd.nVersion = 1;																						  \n"
"pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;								  \n"
"pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;								  \n"
"pfd.iPixelType = PFD_TYPE_RGBA;																		  \n"
"pfd.cColorBits = 24;																					  \n"
"pfd.cDepthBits = 16;																					  \n"
"pfd.iLayerType = PFD_MAIN_PLANE;																		  \n"
"																										  \n"
"iFormat = ChoosePixelFormat(hDC, &pfd);																  \n"
"SetPixelFormat(hDC, iFormat, &pfd);																	  \n"
"HGLRC hRC = wglCreateContext(hDC);																		  \n"
"wglMakeCurrent(hDC, hRC);																				  \n"
"																										  \n"
"MSG message{ 0 };																						  \n"
"																										  \n"
"glEnable(GL_DEPTH_TEST);																				  \n"
"glEnable(GL_STENCIL_TEST);																				  \n"
"glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);																  \n"
"																										  \n"
"glViewport(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));						  \n"
"																										  \n"
"glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)windowWidth / windowHeight, 0.1f, 5000.0f),\n"
"ortho = glm::ortho(0.0f, (GLfloat)windowWidth, (GLfloat)windowHeight, 0.0f, 0.0f, 100.0f);				  \n"
"																										  \n"
"mousePos = new POINT();																				  \n";

	// Вставить:
	// int gameobject_count(5);\n
	file << "int gameobject_count("; file << object_list.size(); file << ");\n";

	// Загрузка .obj файлов
	// Вставить:
	// GameObject **objects = new GameObject*[gameobject_count];\n
	file << "GameObject **objects = new GameObject*[gameobject_count];\n";

	for (int i = 0; i < object_list.size(); i++)
	{
		file << "object[\" + i + \"] = new GameObject(\"" + object_list[i]->path + "\");\n";
	}

	file << ""
"while (true) 																							 \n"
"{																										 \n"
"if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))													  	 \n"
"{																									  	 \n"
"if (message.message == WM_QUIT)																	  	 \n"
"{																								  		 \n"
"break;																						  			 \n"
"}																								  		 \n"
"																										 \n"
"TranslateMessage(&message);																		  	 \n"
"DispatchMessage(&message);																		  		 \n"
"}																									  	 \n"
"																										 \n"
"GetCursorPos(mousePos);																				 \n"
"																										 \n"
"if (mousePos->x >= 0 && mousePos->y >= 0 && mousePos->x <= windowWidth && mousePos->y <= windowHeight)	 \n"
"{																									   	 \n"
"lastMousePosX = mousePos->x;																	  	   	 \n"
"lastMousePosY = mousePos->y;																	  	   	 \n"
"}																									   	 \n"
"																										 \n"
"if (isKeyDown(VK_W))																				   	 \n"
"{																									   	 \n"
"camera.ProcessKeyboard(FORWARD, 0.016f);														  	   	 \n"
"}																									   	 \n"
"																										 \n"
"if (isKeyDown(VK_S))																				   	 \n"
"{																									   	 \n"
"camera.ProcessKeyboard(BACKWARD, 0.016f);														  	   	 \n"
"}																									   	 \n"
"																										 \n"
"if (isKeyDown(VK_A))																				   	 \n"
"{																									   	 \n"
"camera.ProcessKeyboard(LEFT, 0.016f);															  	   	 \n"
"}																									   	 \n"
"																										 \n"
"if (isKeyDown(VK_D))																				   	 \n"
"{																									   	 \n"
"camera.ProcessKeyboard(RIGHT, 0.016f);															  	   	 \n"
"}																									   	 \n"
"																										 \n"
"if (isKeyDown(VK_ESCAPE))																			   	 \n"
"{																									   	 \n"
"PostQuitMessage(0);																				   	 \n"
"}																									   	 \n"
"																										 \n"
"mouseOffsetX = mousePos->x - lastMousePosX;															 \n"
"mouseOffsetY = lastMousePosY - mousePos->y;															 \n"
"																										 \n"
"lastMousePosX = mousePos->x;																		   	 \n"
"lastMousePosY = mousePos->y;																		   	 \n"
"																										 \n"
"camera.ProcessMouseMovement(mouseOffsetX, mouseOffsetY);											   	 \n"
"																									   	 \n"
"glClearColor(1.f, 1.f, 1.f, 1.f);																	   	 \n"
"glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);						   	 \n"
"																										 \n"
"glm::mat4 view = glm::mat4(1.0f);																	   	 \n"
"view = camera.GetViewMatrix();																		   	 \n";

	// Вставляем:
	/*
	for i in objects
	{
		for j in i.scripts
		{
			i.j.start();
		}
	}
	*/

	// Метка прорисовки ВСЕХ объектов
	//for (int i = 0; i < (int)object_list.size(); i++)
	//{
	//	object_list[i]->Draw(projection, view, camera->Position);
	//}

	// Метка прорисовки всех объектов
	file << "for (int i = 0; i < gameobject_count; ++i)\n";
	file << "{\n";
	file << "\objects[i]->Draw(projection, view, camera.Position);\n";
	file << "}\n";

	file << ""
"SwapBuffers(hDC);  \n"
"}					\n";

	// Удаление файлов
	for (int i = 0; i < object_list.size(); i++)
	{
		// Вставляем:
		// "delete"
		str = i;
		file << "delete objects[" + str + "];\n";
	}

	file << "delete[] objects;\n";

	file << ""
"delete mousePos;		    \n"
"							\n"
"wglMakeCurrent(NULL, NULL);\n"
"wglDeleteContext(hRC);	    \n"
"ReleaseDC(hWndEngine, hDC);\n"
"							\n"
"return 0;				    \n"
"}						    \n";

	file.close();

 	return 0;
}
