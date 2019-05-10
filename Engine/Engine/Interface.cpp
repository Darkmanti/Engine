#include <windows.h>
#include <stdint.h>
#include <string>
#include <Commctrl.h>
#pragma comment(lib,"Comctl32.Lib")

#include "GLEW/glew.h"

// Первый define относится к загрузке шрифтов, второй к картинкам
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include <STB/stb_image.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include "Camera.h"
#include "Shader.h"
#include "GameObject.h"
#include "FontObject.h"
#include "Location.h"
#include "Project.h"
#include "Config.h"
#include "Compilation.h"

uint16_t				numberOfKeys;

WNDCLASSEX				pWndEngineClassEx{};				// Структура класса окна

HDC						hDC;							// Дескриптор устройства
HGLRC					hRC;							// Дескпритор

HWND					hWndEngine;						// Главное окно редактора

bool					isLoaded;						// Загружен ли интерфейс

int64_t					mouseOffsetX,
						mouseOffsetY,
						lastMousePosX,
						lastMousePosY;

POINT					*mousePos;

bool					isCameraAction;

HINSTANCE				hInstance;

bool					*previousKeyboardState;

Shader					*ourShader,						// Шейдер
						*fontShader,					// Шейдер
						*selectShader;					// Шейдер

FontObject				*font1;							// Шрифт вывода текста на экран

GLfloat					deltaTime;

Camera					*camera;

// Нет extern'ов
glm::mat4				projection,
						ortho;


bool					show_demo_window,
						show_another_window;

ImVec4					*clear_color;

#define VK_0						0x30
#define VK_1						0x31
#define VK_2						0x32
#define VK_3						0x33
#define VK_4						0x34
#define VK_5						0x35
#define VK_6						0x36
#define VK_7						0x37
#define VK_8						0x38
#define VK_9						0x39

#define VK_A						0x41
#define VK_B						0x42
#define VK_C						0x43
#define VK_D						0x44
#define VK_E						0x45
#define VK_F						0x46
#define VK_G						0x47
#define VK_H						0x48
#define VK_I						0x49
#define VK_J						0x4A
#define VK_K						0x4B
#define VK_L						0x4C
#define VK_M						0x4D
#define VK_N						0x4E
#define VK_O						0x4F
#define VK_P						0x50
#define VK_Q						0x51
#define VK_R						0x52
#define VK_S						0x53
#define VK_T						0x54
#define VK_U						0x55
#define VK_V						0x56
#define VK_W						0x57
#define VK_X						0x58
#define VK_Y						0x59
#define VK_Z						0x5A

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool isKeyDown(int key);

void InitVarInterface()
{
	glewExperimental = GL_TRUE;
	glewInit();

	mousePos = new POINT();

	numberOfKeys = 256;

	previousKeyboardState = new bool[numberOfKeys];
	for (uint16_t keyNum = 0; keyNum < numberOfKeys; ++keyNum)
	{
		previousKeyboardState[keyNum] = isKeyDown(keyNum);
	}

	ourShader = new Shader("Shader//shader.vs", "Shader//shader.fs");
	fontShader = new Shader("Shader//FontShader.vs", "Shader//FontShader.fs");
	selectShader = new Shader("Shader//SelectShader.vs", "Shader//SelectShader.fs");

	font1 = new FontObject(fontShader, 32, 256, "Resource/OpenSans-Regular.ttf", 32, 512, 512);

	deltaTime = 0.016f;

	camera = new Camera(glm::vec3(0.0f, 0.0f, 15.0f));

	projection = glm::mat4();
	ortho = glm::mat4();

	show_demo_window = true;
	show_another_window = false;
	clear_color = new ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

void UninitVarInterface()
{
	delete mousePos;

	delete[] previousKeyboardState;

	delete ourShader;
	delete fontShader;
	delete selectShader;

	delete font1;

	delete camera;

	delete clear_color;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!isLoaded)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
	{
		return true;
	}

	switch (uMsg)
	{
	case WM_MOVE:
		windowX = LOWORD(lParam);
		windowY = HIWORD(lParam);
		break;

	case WM_SIZE:
		if (wParam == SIZE_MAXIMIZED)
		{
			fullscreen = 1;
		}
		else if (wParam == SIZE_RESTORED)
		{
			fullscreen = 0;
		}
		else if (wParam == SIZE_MINIMIZED)
		{
			break;
		}

		windowWidth = LOWORD(lParam);
		windowHeight = HIWORD(lParam);

		glViewport(0, 0, windowWidth, windowHeight);

		projection = glm::perspective(camera->Zoom, (GLfloat)windowWidth / windowHeight, 0.1f, 5000.0f);
		ortho = glm::ortho(0.0f, (GLfloat)windowWidth, (GLfloat)windowHeight, 0.0f, 0.0f, 100.0f);

		/*
		"x = "); Debug(std::to_string(windowRenderRect.left).c_str()); Debug("\n");
		Debug("y = "); Debug(std::to_string(windowRenderRect.top).c_str()); Debug("\n");

		Debug("width = "); Debug(std::to_string(windowRenderRect.right).c_str()); Debug("\n");
		Debug("height = "); Debug(std::to_string(windowRenderRect.bottom).c_str()); Debug("\n");*/
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

// Инициализация интерфейса
uint8_t InitWindow(HINSTANCE *hInstance)
{
	// Описываем поля структур
	pWndEngineClassEx.cbSize = sizeof(WNDCLASSEX);								// Размер в байтах структуры класса
	pWndEngineClassEx.style = CS_VREDRAW | CS_HREDRAW;							// Стиль окна
	pWndEngineClassEx.lpfnWndProc = WindowProc;								// Указатель на оконную процедуру
	pWndEngineClassEx.hInstance = *hInstance;									// Дескриптор приложения
	pWndEngineClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);					// Подгружам курсор
	pWndEngineClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);				// Указатель на кисть с цветом фона (Типо кисть - рисование)
	pWndEngineClassEx.lpszClassName = "WndEngineClass";							// Наименование класса
	pWndEngineClassEx.hIcon = LoadIcon(*hInstance, "IDI_ENGINEICON");			// Иконка

	if (int8_t iError = RegisterClassEx(&pWndEngineClassEx))
	{

	}
	else
	{
		MessageBox(NULL, "Класс не зарегистрирован", "RegisterWindowEngine", MB_OK);
		return iError;
	}

	// Дескриптор окна
	hWndEngine = CreateWindowEx(WS_EX_ACCEPTFILES,											// Extended style
				pWndEngineClassEx.lpszClassName,											// Название класса
				"Движок",																	// Название окна
				WS_OVERLAPPEDWINDOW,														// Стиль окна
				windowX, windowY,															// Позиция
				windowWidth, windowHeight,													// Размер
				0,																			// Родительское окно
				0,																			// Меню
				*hInstance,																	// Десприптор приложения
				0																			// Все это говно доступно на msdn
	);

	// Проверка на создание окна
	if (!hWndEngine)
	{
		MessageBox(NULL, "Окно не создано", "Ошибка", MB_OK);
		return 1;
	}

	if (!fullscreen)
		if (ShowWindow(hWndEngine, SW_NORMAL))
		{
			return 2;
		}
		else;
	else
		if (ShowWindow(hWndEngine, SW_MAXIMIZE))
		{
			return 2;
		}

	if (!UpdateWindow(hWndEngine))
	{
		return 3;
	}

	return 0;
}

// Инициализация параметров интерфейса imgui
void InitImgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWndEngine);
	ImGui_ImplOpenGL3_Init("#version 330");
}

bool isKeyDown(int key)
{
	return (GetAsyncKeyState(key) & (1 << 16));
}

bool isKeyFirstPressed(int key)
{
	bool previousState = previousKeyboardState[key];

	previousKeyboardState[key] = isKeyDown(key);

	return (previousKeyboardState[key] && !previousState);
}

bool isKeyFirstReleased(int key)
{
	bool previousState = previousKeyboardState[key];

	previousKeyboardState[key] = isKeyDown(key);

	return (!previousKeyboardState[key] && previousState);
}

void CameraControllAction()
{
	GetCursorPos(mousePos);

	if (mousePos->x >= windowX && mousePos->y >= windowY && mousePos->x <= windowX + windowWidth && mousePos->y <= windowY + windowHeight)
	{
		// Если нажатие клавиш управления происходит впервые
		if (isKeyFirstPressed(VK_RBUTTON) || isKeyFirstPressed(VK_LBUTTON) && isKeyDown(VK_LMENU))
		{
			isCameraAction = true;

			lastMousePosX = mousePos->x;
			lastMousePosY = mousePos->y;
		}
	}

	if ((isKeyDown(VK_RBUTTON) || isKeyDown(VK_LBUTTON) && isKeyDown(VK_LMENU)) && isCameraAction)
	{
		if (isKeyDown(VK_W))
		{
			camera->ProcessKeyboard(FORWARD, deltaTime);
		}

		if (isKeyDown(VK_S))
		{
			camera->ProcessKeyboard(BACKWARD, deltaTime);
		}

		if (isKeyDown(VK_A))
		{
			camera->ProcessKeyboard(LEFT, deltaTime);
		}

		if (isKeyDown(VK_D))
		{
			camera->ProcessKeyboard(RIGHT, deltaTime);
		}

		mouseOffsetX = mousePos->x - lastMousePosX;
		mouseOffsetY = lastMousePosY - mousePos->y;

		//Debug("mousepos.x = "); Debug(std::to_string(mousePos->x).c_str()); Debug("\t");
		//Debug("last.x = "); Debug(std::to_string(lastMousePosX).c_str()); Debug("\t");
		//Debug("offset.x = "); Debug(std::to_string(mouseOffsetX).c_str()); Debug("\t");

		//Debug("\n");

		//Debug("mousepos.y = "); Debug(std::to_string(mousePos->y).c_str()); Debug("\t");
		//Debug("last.y = "); Debug(std::to_string(lastMousePosY).c_str()); Debug("\t");
		//Debug("offset.y = "); Debug(std::to_string(mouseOffsetY).c_str()); Debug("\t");

		//Debug("\n");

		lastMousePosX = mousePos->x;
		lastMousePosY = mousePos->y;

		camera->ProcessMouseMovement(mouseOffsetX, mouseOffsetY);
	}
	else
	{
		isCameraAction = false;
	}
}

// Метод с циклом программы
void Loop()
{
	MSG message{ 0 }; 	// Структура сообщения к окну

	// Тест трафарета и глубины
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// Типо выбрали бревно - пока не работает как надо и это доработается когда будут меши
	//object3.isSelect = true;

	// Цвет фона
	clear_color->x = 0.2f;
	clear_color->y = 0.3f;
	clear_color->z = 0.3f;

	OpenProj();
	Compilation();

	// Пока есть сообщения
	// Если система вернула отрицательный код (ошибка), то выходим из цикла обработки
	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}

			// Обрабатываем сообщения в WndProc
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		CameraControllAction();

		glClearColor(clear_color->x, clear_color->y, clear_color->z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		if (isKeyFirstReleased(VK_G))
		{
			//AddGameObject("Resource/denis");
			AddGameObject("Resource/city");
			//AddGameObject("Resource/toilet");
		}

		/*GLfloat currentFrame = GetProcessTimes(); НУЖНО ВЗЯТЬ ВРЕМЯ РАБОТЫ!!!
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;*/

		glm::mat4 view = glm::mat4(1.0f);
		view = camera->GetViewMatrix();

		for (int i = 0; i < (int)object_list.size(); i++)
		{
			object_list[i]->Draw(projection, view, camera->Position);
		}

		font1->Print(0, 20, "Player", glm::vec3(1.0f, 1.0f, 1.0f), ortho);
		font1->Print(200, 20, "Health: 100", glm::vec3(1.0f, 0.0f, 0.0f), ortho);
		font1->Print(400, 20, "Armor: 100", glm::vec3(0.0f, 0.0f, 1.0f), ortho);

		// Интерфейс отрисовка
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &(camera->MovementSpeed), 10.0f, 200.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		{
			counter++;
		}

		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");

			if (ImGui::Button("Close Me"))
			{
				show_another_window = false;
			}

			ImGui::End();
		}

		// Вызов рендера интерфейса
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// Интерфейс отрисовка

		SwapBuffers(hDC);
	}
}
