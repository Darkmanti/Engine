#include <windows.h>
#include <stdint.h>
#include <string>
#include <Commctrl.h>
#pragma comment(lib,"Comctl32.Lib")

#include "GLEW/glew.h"

#include "Project.h"
#include "Location.h"
#include "Shader.h"

// ������ define ��������� � �������� �������, ������ � ���������
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include <STB/stb_image.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include "Camera.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include "GameObject.h"
#include "FontObject.h"

namespace WinApi
{
	WNDCLASSEX			pWndEngineClassEx;				// ��������� ������ ����

// ��������� �����������
	HDC					hDC;							// ���������� ����������
	HGLRC				hRC;							// ����������

	HWND				hWndEngine;						// ������� ���� ���������

	HANDLE				debugConsole;

	OPENFILENAME		OFN{ 0 };
	LPSTR				filePath;

	char				szDirect[MAX_PATH],
		szFileName[MAX_PATH];

	bool				isLoaded;

	int64_t				mouseOffsetX,
		mouseOffsetY,
		lastMousePosX,
		lastMousePosY;

	POINT				mousePos{};

	RECT				windowRenderRect{},
		windowEngineRect{};

	bool				isCameraAction;

	HINSTANCE			hInstance;

	extern const uint16_t NumberOfKeys = 256;

	bool					previousKeyboardState[NumberOfKeys];

	std::string				lastProjectFileName = "MyProject",
		lastLocationFileName = "MyLocation",
		projectPath;

	Shader					ourShader("Shader//shader.vs", "Shader//shader.fs"),
		fontShader("Shader//FontShader.vs", "Shader//FontShader.fs"),
		selectShader("Shader//SelectShader.vs", "Shader//SelectShader.fs");

	//������������� ������
	FontObject				font1(&fontShader, 32, 256, "Resource/OpenSans-Regular.ttf", 32, 512, 512);

	GLuint					texture1,
		texture2,
		texture3,
		texture4,
		texture5,
		denisjpg;

	GLfloat					deltaTime = 0.016f;
	Camera					camera(glm::vec3(0.0f, 0.0f, 15.0f));

	void loadImage(GLuint &texture, char const* fileName, int Par)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		stbi_set_flip_vertically_on_load(true);
		GLint w, h, comp;
		unsigned char* image = stbi_load(fileName, &w, &h, &comp, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, Par, w, h, 0, Par, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(image);
	}

	// ====

	extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// ������� ���������
	LRESULT WndEngineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (!isLoaded)
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		{
			return true;
		}

		switch (message)
		{
		case WM_COMMAND:
			break;
		case WM_MOVE:
		case WM_SIZE:
			GetWindowRect(hWndEngine, &windowEngineRect);

			glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));

			/*Debug("x = "); Debug(std::to_string(windowRenderRect.left).c_str()); Debug("\n");
			Debug("y = "); Debug(std::to_string(windowRenderRect.top).c_str()); Debug("\n");

			Debug("width = "); Debug(std::to_string(windowRenderRect.right).c_str()); Debug("\n");
			Debug("height = "); Debug(std::to_string(windowRenderRect.bottom).c_str()); Debug("\n");*/
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	// ����������� ������ ����
	ATOM RegisterWindowEngine()
	{
		// ��������� ���� ��������
		pWndEngineClassEx.cbSize = sizeof(WNDCLASSEX);								// ������ � ������ ��������� ������
		pWndEngineClassEx.style = CS_VREDRAW | CS_HREDRAW;							// ����� ����
		pWndEngineClassEx.lpfnWndProc = WndEngineProc;								// ��������� �� ������� ���������
		pWndEngineClassEx.hInstance = hInstance;									// ���������� ����������
		pWndEngineClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);					// ��������� ������
		pWndEngineClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);				// ��������� �� ����� � ������ ���� (���� ����� - ���������)
		pWndEngineClassEx.lpszClassName = "WndEngineClass";							// ������������ ������
		pWndEngineClassEx.hIcon = LoadIcon(hInstance, "IDI_ENGINEICON");			// ������

		if (int16_t iError = RegisterClassEx(&pWndEngineClassEx))
		{

		}
		else
		{
			MessageBox(NULL, "����� �� ���������������", "RegisterWindowEngine", MB_OK);
			return iError;
		}

		return 0;
	}

	// �������� ����
	uint8_t CreateWindowEngine()
	{
		// ���������� ����
		hWndEngine = CreateWindowEx(WS_EX_ACCEPTFILES,									// Extended style
			pWndEngineClassEx.lpszClassName,											// �������� ������
			"������",																	// �������� ����
			WS_OVERLAPPEDWINDOW,														// ����� ����
			0, 0,																		// �������
			1366, 768,																	// ������
			0,																			// ������������ ����
			0,																			// ����
			hInstance,																	// ���������� ����������
			0																			// ��� ��� ����� �������� �� msdn
		);

		// �������� �� �������� ����
		if (!hWndEngine)
		{
			MessageBox(NULL, "���� �� �������", "������", MB_OK);
			return 1;
		}

		return 0;
	}

	// ������������� ����������
	uint8_t InitInterface()
	{
		uint16_t iError;

		if (iError = RegisterWindowEngine()) return iError * 10 + 1;

		// �������� �� �������� ����
		if (iError = CreateWindowEngine()) return iError * 10 + 5;

		return 0;
	}

	// ������������� ���������� ����������
	uint8_t InitImgui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(hWndEngine);
		ImGui_ImplOpenGL3_Init("#version 330");

		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	}

	uint8_t ShowInterface(const int16_t nCmdShow)
	{
		if (ShowWindow(hWndEngine, SW_MAXIMIZE)) return 2;
		if (!UpdateWindow(hWndEngine)) return 3;

		return 0;
	}

	void InitInput()
	{
		for (uint16_t keyNum = 0; keyNum < NumberOfKeys; ++keyNum)
		{
			previousKeyboardState[keyNum] = isKeyDown(keyNum);
		}
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
		GetCursorPos(&mousePos);

		if (mousePos.x >= windowRenderRect.left && mousePos.y >= windowRenderRect.top && mousePos.x <= windowRenderRect.right && mousePos.y <= windowRenderRect.bottom)
		{
			// ���� ������� ������ ���������� ���������� �������
			if (isKeyFirstPressed(VK_RBUTTON) || isKeyFirstPressed(VK_LBUTTON) && isKeyDown(VK_LMENU))
			{
				isCameraAction = true;

				lastMousePosX = mousePos.x;
				lastMousePosY = mousePos.y;
			}
		}

		if ((isKeyDown(VK_RBUTTON) || isKeyDown(VK_LBUTTON) && isKeyDown(VK_LMENU)) && isCameraAction)
		{
			if (isKeyDown(VK_W))
			{
				camera.ProcessKeyboard(FORWARD, deltaTime);
			}

			if (isKeyDown(VK_S))
			{
				camera.ProcessKeyboard(BACKWARD, deltaTime);
			}

			if (isKeyDown(VK_A))
			{
				camera.ProcessKeyboard(LEFT, deltaTime);
			}

			if (isKeyDown(VK_D))
			{
				camera.ProcessKeyboard(RIGHT, deltaTime);
			}

			mouseOffsetX = mousePos.x - lastMousePosX;
			mouseOffsetY = lastMousePosY - mousePos.y;

			//Debug("mousepos.x = "); Debug(std::to_string(mousePos.x).c_str()); Debug("\t");
			//Debug("last.x = "); Debug(std::to_string(lastMousePosX).c_str()); Debug("\t");
			//Debug("offset.x = "); Debug(std::to_string(mouseOffsetX).c_str()); Debug("\t");

			//Debug("\n");

			//Debug("mousepos.y = "); Debug(std::to_string(mousePos.y).c_str()); Debug("\t");
			//Debug("last.y = "); Debug(std::to_string(lastMousePosY).c_str()); Debug("\t");
			//Debug("offset.y = "); Debug(std::to_string(mouseOffsetY).c_str()); Debug("\t");

			//Debug("\n");

			lastMousePosX = mousePos.x;
			lastMousePosY = mousePos.y;

			camera.ProcessMouseMovement(mouseOffsetX, mouseOffsetY);
		}
		else
		{
			isCameraAction = false;
		}
	}

	void Debug(const char *sms)
	{
		WriteConsole(debugConsole, sms, strlen(sms), nullptr, NULL);
	}

	void Debug(int str)
	{
		char sms[256];
		_itoa(str, sms, 10);
		WriteConsole(debugConsole, sms, strlen(sms), nullptr, NULL);
	}

	void Debug(unsigned int str)
	{
		char sms[256];
		_itoa(str, sms, 10);
		WriteConsole(debugConsole, sms, strlen(sms), nullptr, NULL);
	}

	void Debug(float str)
	{
		char sms[256];
		std::stringstream iostr;
		iostr << str;
		iostr >> sms;
		WriteConsole(debugConsole, sms, strlen(sms), nullptr, NULL);
	}

	void Clear()
	{
		system("cls");
	}

	// ����� � ������ ���������
	void Loop()
	{
		InitImgui();

		MSG message{ 0 }; 	// ��������� ��������� � ����

		// �������� ����� ����� ������������� �������� ����� ===========================================
		glewExperimental = GL_TRUE;
		glewInit();

		// ���� ��������� � �������
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		loadImage(texture1, "Resource/container.jpg", GL_RGB);
		loadImage(texture2, "Resource/container2.png", GL_RGB);
		loadImage(texture3, "Resource/Wood/wood.jpg", GL_RGB);
		loadImage(texture4, "Resource/Iron/iron.jpg", GL_RGB);
		loadImage(texture5, "Resource/Bereza/Bereza.png", GL_RGBA);
		loadImage(denisjpg, "Resource/denis/denis.jpg", GL_RGB);

		// ������������� ��������
		GameObject denis(&ourShader, "D:/Engine/Engine/Engine/Resource/denis");
		denis.setModel(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(-20.0f, -20.0f, 20.0f), 9.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		// ���� ������� ������ - ���� �� �������� ��� ���� � ��� ������������ ����� ����� ����
		//object3.isSelect = true;

		// �������
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(camera.Zoom, (GLfloat)800 / (GLfloat)600, 0.1f, 1000.0f);
		glm::mat4 ortho(1.0f);
		ortho = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, 0.0f, 100.0f);

		// �������� ����� ����� ������������� �������� ����� ===========================================

		Debug("Initialisation succes\n");

		// ���� ���� ���������
		// ���� ������� ������� ������������� ��� (������), �� ������� �� ����� ���������
		while (true)
		{
			if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				if (message.message == WM_QUIT)
				{
					break;
				}

				// ������������ ��������� � WndProc
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			CameraControllAction();

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			if (isKeyDown(VK_G))
			{
				AddGameObject();
			}

			// ��������� ���������� =================================================================
			/*GLfloat currentFrame = GetProcessTimes(); ����� ����� ����� ������!!!
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;*/

			glm::mat4 view = glm::mat4(1.0f);
			view = camera.GetViewMatrix();

			for (int i = 0; i < gameobject_count; i++)
			{
				object_list[i].Draw(projection, view, camera.Position);
			}

			glBindTexture(GL_TEXTURE_2D, denisjpg);
			denis.Draw(projection, view, camera.Position);
			glBindTexture(GL_TEXTURE_2D, 0);

			font1.Print(0, 0, "Player", glm::vec3(1.0f, 1.0f, 1.0f), ortho);
			font1.Print(0, 200, "Health: 100", glm::vec3(1.0f, 0.0f, 0.0f), ortho);
			font1.Print(0, 400, "Armor: 100", glm::vec3(0.0f, 0.0f, 1.0f), ortho);
			// ��������� ���������� =================================================================


			// ��������� ��������� =========================================================================================
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
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
					show_another_window = false;
				ImGui::End();
			}

			// ����� ������� ����������
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			// ��������� ��������� =========================================================================================

			SwapBuffers(hDC);
		}
	}
}