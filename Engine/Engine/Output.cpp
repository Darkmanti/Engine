#include "Output.h"

#include "GLEW/glew.h"

#include <windows.h>
#include <stdint.h>

#include "Engine.h"
#include "Input.h"

namespace Output
{
	 GLuint			windowWidth;			// ������ ���� ���������
	 GLuint			windowHeight;			// ������ ���� ���������

	 GLuint			windowRenderWidth;		// ������ ���� �������. ���������� ������������ �������
	 GLuint			windowRenderHeight;		// ������ ���� �������. ���������� ������������ �������


	 WNDCLASSEX		pWndClassEx;			// ��������� ������ ����


	 GLuint			vArrayObject;			// VAO
	 GLuint			vBufferObject;			// VBO
	 GLuint			shaderProgram;			// ��������� ���������


	// ��������� �����������
	 HDC			hDC;						// ���������� ����������
	 HGLRC			hRC;						// ���������� ...

	 HWND hWnd;									// ������� ���� ���������
	 HWND hWindowRender;						// ���� ������� ������ ���������

	// ����������� ������
	ATOM RegisterEngineWindow(const HINSTANCE hInstance)
	{
		// ��������� ���� ��������
		pWndClassEx.cbSize = sizeof(WNDCLASSEX),						// ������ � ������ ��������� ������
		pWndClassEx.style = CS_VREDRAW | CS_HREDRAW;					// ����� ����
		pWndClassEx.lpfnWndProc = WndProc;								// ��������� �� ������� ���������
		pWndClassEx.hInstance = hInstance;								// ���������� ����������
		pWndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);				// ��������� ������
		pWndClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);		// ��������� �� ����� � ������ ���� (���� ����� - ���������)
		pWndClassEx.lpszClassName = "EngineWndClass\0";					// ������������ ������

		int16_t iError(RegisterClassEx(&pWndClassEx));

		// �������� �� ���������� ����������� ����
		if (!iError)
		{
			MessageBox(NULL, "����� �� ���������������", "������", MB_OK);
			return 0;
		}

		return iError;
	}

	// �������� ����
	uint8_t CreateEngineWindow(const int32_t nCmdShow)
	{
		// ���������� ����
		HWND &hWnd(Output::hWnd);
		hWnd = CreateWindowEx(
			WS_EX_ACCEPTFILES | WS_EX_WINDOWEDGE,	// Extended style
			Output::pWndClassEx.lpszClassName,				// �������� ������
			static_cast<LPCTSTR>("Engine\0"),		// �������� ����
			WS_SYSMENU | WS_CAPTION | WS_SIZEBOX |
			WS_MINIMIZEBOX | WS_MAXIMIZEBOX,		// ����� ����
			0, 0,									// �������
			windowWidth, windowHeight,				// ������
			0,										// ��� �����-�� ������
			0,										// � ��� ���-�� ���
			Engine::hInstance,						// ���������� ����������
			0										// ��� ��� ����� �������� �� msdn
		);

		// �������� �� �������� ����
		if (!hWnd)
		{
			MessageBox(hWnd, "������", "���� �� �������", MB_OK);
			return 0;
		}

		// ���������� ��������� ��������� (������, ������, inputText � ��.)
		if (InitInterface())
		{
			MessageBox(hWnd, "������", "��������� �� ������", MB_OK);

			return 0;
		}

		// �������� ����
		ShowWindow(hWnd, nCmdShow);

		// ��������� ����
		UpdateWindow(hWnd);

		return 1;
	}

	// ������� ����������� ��������� (��������� ����)
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND:
			if (lParam == (LPARAM)Input::hBtn_OpenModel)
			{
				//// ��������� ������� ������
				//GLdouble *vertices;

				//std::ifstream file;

				//file.open(getDirAppData() + static_cast<std::string>("\\models\\cube.obj"));

				//if (!file.is_open())
				//{
				//	break;
				//}

				//std::string str;

				//std::string::size_type size;

				//file >> str;

				//uint64_t countVertices(0); //���������� ��������� ��� 
				//countVertices = std::stoul(str, &size) * 3;

				//vertices = new double[countVertices];

				//uint64_t	countVertex(0);		// �������� ���������� ���������
				//uint8_t		countEmptyLines(0); // ���������� ����������� �����

				//while (!file.eof())
				//{
				//	file >> str;

				//	if (str[0] == '#')
				//	{
				//		getline(file, str);
				//		countEmptyLines = 0;
				//		continue;
				//	}

				//	if (str[0] == 'v' && str[1] != 'n')
				//	{
				//		double x, y, z;

				//		file >> str;
				//		x = std::stod(str, &size);

				//		file >> str;
				//		y = std::stod(str, &size);

				//		file >> str;
				//		z = std::stod(str, &size);

				//		vertices[countVertex] = x; ++countVertex;
				//		vertices[countVertex] = y; ++countVertex;
				//		vertices[countVertex] = z; ++countVertex;

				//		countEmptyLines = 0;
				//		continue;
				//	}

				//	if (countEmptyLines >= 8)
				//	{
				//		break;
				//	}

				//	getline(file, str);

				//	++countEmptyLines;
				//}

				//file.close();

				//��������
				GLfloat vertices[]{
					-1.f, -1.f, 0.f,
					1.f, -1.f, 0.f,
					0.f,  1.f, 0.f
				};

				glGenVertexArrays(1, &vArrayObject);
				glBindVertexArray(vArrayObject);

				glGenBuffers(1, &vBufferObject); // ������������� ID ������� ������
				glBindBuffer(GL_ARRAY_BUFFER, vBufferObject); // �������� ������
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // GL_ARRAY_BUFFER, 

				// ������ ������ ������ ����� ����� ��������� ���������
				glEnableVertexAttribArray(0); // ���������, ��� ������ ������� ��������� ����� �������
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
				//glBindVertexArray(0);

				//delete	[]vertices;
			}
			break;

		case WM_LBUTTONUP: // ������� �� ���������
			MessageBox(hWnd, "�� ��������!", "�������", MB_OK);
			break;

		case WM_SIZE: // ������� �� ����� ������� ����
			MoveWindow(hWindowRender, 0, Input::sizeButton, LOWORD(lParam), HIWORD(lParam), true);
			Output::windowWidth = LOWORD(lParam);
			Output::windowHeight = HIWORD(lParam);
			Output::windowRenderWidth = LOWORD(lParam);
			Output::windowRenderHeight = HIWORD(lParam) - Input::sizeButton;
			break;

		case WM_DESTROY: // ������� �� ���������
			PostQuitMessage(0);
			break;

		default: // ��� �������������� ��������� ���������� ���� Windows
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	// ����� � ������ ���������
	void Loop()
	{
		MSG message{ 0 }; 	// ��������� ��������� � ����
		int8_t iResult;		// ��� ���������

		// ���� ���� ���������
		while ((iResult = GetMessage(&message, NULL, 0, 0)))
		{
			// ���� ������� ������� ������������� ��� (������), �� ������� �� ����� ���������
			if (iResult < 0) break;

			// ����� ������������ ��������� � WndProc
			TranslateMessage(&message);
			DispatchMessage(&message);

			glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			glUseProgram(shaderProgram);
			glBindBuffer(GL_ARRAY_BUFFER, vBufferObject);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLES, 0, 3);
			SwapBuffers(Output::hDC);
		}
	}

	// ������������� ����������
	int16_t InitInterface()
	{
		// ViewportRender
		{
			HWND &hBtn(hWindowRender);
			hBtn = CreateWindow("BUTTON", "",
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				0, Input::sizeButton, windowRenderWidth, windowRenderHeight, hWnd, 0, Engine::hInstance, NULL);

			// ������� ������ ���������� ������� �� ������
			if (!hBtn)
			{
				return 1;
			}

			ShowWindow(hBtn, SW_NORMAL);
		}

		// ������ �������� ������
		{
			HWND &hBtn(Input::hBtn_OpenModel);
			hBtn = CreateWindow("BUTTON", "������� ������",
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				0, 0, Input::sizeButton, Input::sizeButton, hWnd, 0, Engine::hInstance, NULL);

			if (!hBtn)
			{
				return 2;
			}

			ShowWindow(hBtn, SW_NORMAL);
		}

		{

		}

		return 0;
	}

	void EnableOpenGL()
	{
		PIXELFORMATDESCRIPTOR pfd;

		int iFormat;

		hDC = GetDC(hWindowRender);

		ZeroMemory(&pfd, sizeof(pfd));

		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 16;
		pfd.iLayerType = PFD_MAIN_PLANE;
		iFormat = ChoosePixelFormat(hDC, &pfd);
		SetPixelFormat(hDC, iFormat, &pfd);

		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
	}

	void DisableOpenGL()
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
		ReleaseDC(hWnd, hDC);
	}
};
