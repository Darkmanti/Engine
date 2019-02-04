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
		pWndClassEx.hIcon = LoadIcon(hInstance, "IDI_ENGINEICON");		// ������

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
			Output::pWndClassEx.lpszClassName,		// �������� ������
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
			switch (LOWORD(wParam))
			{
			case MENU_FILE_NEWLOCATION:
				//
				break;
			case MENU_FILE_SAVELOCATION:
				//
				break;
			case MENU_FILE_SAVEASLOCATION:
				//
				break;
			case MENU_FILE_QUIT:
				//
				break;


			case MENU_PROJECT_IMPORT_MODEL:
				//
				break;
			case MENU_PROJECT_IMPORT_TEXTURE:
				//
				break;
			}
			
			if (lParam == (LPARAM)Input::hBtn_Shader)
			{
				// ��������� ���������
				shaderProgram = glCreateProgram();

				// �������
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
						MessageBox(NULL, "���������� ������ �� ���������������" + *infoLog, "������", MB_ICONERROR | MB_OK);
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
						MessageBox(NULL, "����������� ������ �� ���������������" + *infoLog, "������", MB_ICONERROR | MB_OK);
					}

					glAttachShader(Output::shaderProgram, vertexShaderID);
					glAttachShader(Output::shaderProgram, fragmentShaderID);
					glLinkProgram(Output::shaderProgram);


					glGetProgramiv(Output::shaderProgram, GL_COMPILE_STATUS, &iError);

					if (!iError)
					{
						GLchar infoLog[512];
						glGetProgramInfoLog(Output::shaderProgram, 512, NULL, infoLog);
						MessageBox(NULL, "��������� ��������� �� �������" + *infoLog, "������", MB_ICONERROR | MB_OK);
					}

					glUseProgram(Output::shaderProgram);

					glDeleteShader(vertexShaderID);
					glDeleteShader(fragmentShaderID);
				}

				glUseProgram(shaderProgram);
			}
			else if (lParam == (LPARAM)Input::hBtn_OpenModel)
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
			}
			else if (lParam == (LPARAM)Input::hBtn_ShowModel)
			{
				//��������
				GLfloat *vertices = new GLfloat[9]{
					-1.f, -1.f, 0.f,
					1.f, -1.f, 0.f,
					0.f,  1.f, 0.f
				};

				//glGenVertexArrays(1, &vArrayObject);
				//glBindVertexArray(vArrayObject);

				glGenBuffers(1, &vBufferObject); // ������������� ID ������� ������
				glBindBuffer(GL_ARRAY_BUFFER, vBufferObject); // �������� ������
				glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // GL_ARRAY_BUFFER, 

				// ������ ������ ������ ����� ����� ��������� ���������
				glEnableVertexAttribArray(0); // ���������, ��� ������ ������� ��������� ����� �������
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

				delete[]vertices;
			}
			else if (lParam == (LPARAM)Input::hBtn_CloseModel)
			{
				glDeleteBuffers(1, &Output::vBufferObject);
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

			// ������ � ������� �������
			//glViewport(256, 0, 800, 600);

			// ������ � ������� �������
			glViewport(0, 0, Output::windowRenderWidth, Output::windowRenderHeight);
			break;

		case WM_DESTROY: // ������� �� ���������
			MessageBox(Output::hWnd, "��", "�����", MB_OK);
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
			if (iResult < 0)
			{
				break;
			}

			// ����� ������������ ��������� � WndProc
			TranslateMessage(&message);
			DispatchMessage(&message);

			// �������
			glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// ����� �������
			glDrawArrays(GL_TRIANGLES, 0, 3);

			// ����� ������
			SwapBuffers(Output::hDC);
		}
	}

	// ������������� ����������
	int16_t InitInterface(const int32_t nCmdShow)
	{
		if (!Output::RegisterEngineWindow(Engine::hInstance)) return 1; // �������� �� ���������� ����������� ����
		if (!Output::CreateEngineWindow(nCmdShow)) return 1; // �������� �� ���������� �������� ����

		// ViewportRender
		{
			HWND &hBtn(hWindowRender);
			hBtn = CreateWindow("BUTTON", "���� �������",
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				0, Input::sizeButton, windowRenderWidth, windowRenderHeight, hWnd, 0, Engine::hInstance, NULL);

			// ������� ������ ���������� ������� �� ������
			if (!hBtn)
			{
				return 1;
			}

			ShowWindow(hBtn, SW_NORMAL);
		}

		// ������ ���������� ��������� ���������
		{
			HWND &hBtn(Input::hBtn_OpenModel);
			hBtn = CreateWindow("BUTTON", "�������������� �������",
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				0, 0, Input::sizeButton, Input::sizeButton, hWnd, 0, Engine::hInstance, NULL);

			if (!hBtn)
			{
				return 2;
			}

			ShowWindow(hBtn, SW_NORMAL);
		}

		// ������ �������� ������
		{
			HWND &hBtn(Input::hBtn_OpenModel);
			hBtn = CreateWindow("BUTTON", "������� ������",
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				Input::sizeButton, 0, Input::sizeButton, Input::sizeButton, hWnd, 0, Engine::hInstance, NULL);

			if (!hBtn)
			{
				return 3;
			}
			
			ShowWindow(hBtn, SW_NORMAL);
		}

		// ������ ��������� ������
		{
			HWND &hBtn(Input::hBtn_OpenModel);
			hBtn = CreateWindow("BUTTON", "�������� ������",
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				Input::sizeButton * 2, 0, Input::sizeButton, Input::sizeButton, hWnd, 0, Engine::hInstance, NULL);

			if (!hBtn)
			{
				return 4;
			}

			ShowWindow(hBtn, SW_NORMAL);
		}

		// ������ �������� ������
		{
			HWND &hBtn(Input::hBtn_CloseModel);
			hBtn = CreateWindow("BUTTON", "������� ������",
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				Input::sizeButton * 3, 0, Input::sizeButton, Input::sizeButton, hWnd, 0, Engine::hInstance, NULL);

			if (!hBtn)
			{
				return 5;
			}

			ShowWindow(hBtn, SW_NORMAL);
		}

		// ��������� ����
		Input::hMenu = CreateMenu();
		Input::hPopMenuFile = CreatePopupMenu();
		Input::hPopMenuProject = CreatePopupMenu();
		Input::hPopMenuProjectImport = CreatePopupMenu();

		AppendMenu(Input::hMenu, MF_STRING | MF_POPUP, (UINT)Input::hPopMenuFile, "&����");
		AppendMenu(Input::hPopMenuFile, MF_STRING, MENU_FILE_NEWLOCATION, "����� �������\tCtrl+N");
		AppendMenu(Input::hPopMenuFile, MF_STRING, MENU_FILE_SAVELOCATION, "���������\tCtrl+S");
		AppendMenu(Input::hPopMenuFile, MF_STRING, MENU_FILE_SAVEASLOCATION, "��������� ���\tCtrl+Shift+S");
		AppendMenu(Input::hPopMenuFile, MF_SEPARATOR, MENU_FILE_SAVEASLOCATION + 1, "");
		AppendMenu(Input::hPopMenuFile, MF_STRING, MENU_FILE_SAVEASLOCATION, "�����\tAlt+F4");

		AppendMenu(Input::hMenu, MF_STRING | MF_POPUP, (UINT)Input::hPopMenuProject, "&������");
		AppendMenu(Input::hPopMenuProject, MF_STRING | MF_POPUP, (UINT)Input::hPopMenuProjectImport, "&������");
		AppendMenu(Input::hPopMenuProjectImport, MF_STRING, MENU_PROJECT_IMPORT_MODEL, "������");
		AppendMenu(Input::hPopMenuProjectImport, MF_STRING, MENU_PROJECT_IMPORT_TEXTURE, "��������");

		SetMenu(hWnd, Input::hMenu);

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
