// ���� �������� � ���� ����� ����� ������������� �������� ����
#include "GLEW/glew.h"

#include "globals.h"
#include "interface.h"
#include "buttons.h"

#include <windows.h>
#include <stdint.h>
#include <fstream>
#include <string>

// �������� ������� ����� ��� ����������� �������� ��� ��������� � ��������� WNDCLASSA
LRESULT WndProc(HWND, uint32_t, WPARAM, LPARAM);

// ����������� ���� ��� ������������� ����������� �����������
ATOM RegisterEngineWindow(const HINSTANCE hInstance)
{
	// ��������� ���� ��������
	getWndClassA().style = CS_VREDRAW | CS_HREDRAW;				// ����� ����
	getWndClassA().lpfnWndProc = WndProc;						// ��������� �� ������� ���������
	getWndClassA().hInstance = hInstance;						// ���������� ����������
	getWndClassA().hCursor = LoadCursor(NULL, IDC_ARROW);		// ��������� ������
	getWndClassA().hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);	// ��������� �� ����� � ������ ���� (���� ����� - ���������)
	getWndClassA().lpszClassName = "EngineWndClass\0";			// ������������ ������

	int16_t iError(RegisterClass(&getWndClassA()));

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
	HWND &hWnd(getHWnd());
	hWnd = CreateWindow(
		getWndClassA().lpszClassName,				// �������� ������
		static_cast<LPCTSTR>("Engine\0"),		// �������� ����
		WS_SYSMENU | WS_CAPTION | WS_SIZEBOX |
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX,		// ����� ����
		0, 0,									// �������
		getWindowWidth(), getWindowHeight(),	// ������
		0,										// ��� �����-�� ������
		0,										// � ��� ���-�� ���
		getHInstance(),							// ���������� ����������
		0										// ��� ��� ����� �������� �� msdn
	);

	if (!getHWnd())
	{
		MessageBox(getHWnd(), "������", "���� �� �������", MB_OK);
		return 0;
	}

	if (InitInterface())
	{
		MessageBox(getHWnd(), "������", "��������� �� ������", MB_OK);

		return 0;
	}

	// �������� ����
	ShowWindow(getHWnd(), nCmdShow);

	// ��������� ����
	UpdateWindow(getHWnd());

	return 1;
}

// ������� ����������� ��������� (��������� ����)
LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		if (lParam == (LPARAM)getHBtnOpenModel())
		{
			// ��������� ������� ������
			GLfloat *vertices;

			std::ifstream file;

			file.open(getDirAppData() + static_cast<std::string>("cube.obj"));

			if (!file.is_open())
			{
				break;
			}

			std::string str;

			uint64_t	countVertex(0);		// �������� ���������� ���������
			uint8_t		countEmptyLines(0); // ���������� ����������� �����

			std::string::size_type size;

			file >> str;

			uint64_t countVertices(0); //���������� ��������� ��� 
			countVertices = std::stod(str, &size);

			vertices = new GLfloat[countVertices];

			while (!file.eof())
			{
				file >> str;

				if (str[0] == '#')
				{
					getline(file, str);
					countEmptyLines = 0;
					continue;
				}

				if (str[0] == 'v' && str[1] != 'n')
				{
					double vertex;

					file >> str;
					vertex = std::stod(str, &size);

					vertices[countVertex] = vertex;
					++countVertex;

					countEmptyLines = 0;
					continue;
				}

				if (countEmptyLines >= 8)
				{
					break;
				}

				getline(file, str);

				++countEmptyLines;
			}

			file.close();

			//��������
			//GLfloat vertices[]{
			//	-1.f, -1.f, 0.0f,
			//	1.f, -1.f, 0.0f,
			//	0.0f,  1.f, 0.0f
			//};

			glGenVertexArrays(1, &getVArrayObject());
			glBindVertexArray(getVArrayObject());

			// VBO
			GLuint vBufferObject;

			glGenBuffers(1, &vBufferObject); // ������������� ID ������� ������
			glBindBuffer(GL_ARRAY_BUFFER, vBufferObject); // �������� ������
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // GL_ARRAY_BUFFER, 

			// ������ ������ ������ ����� ����� ��������� ���������
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // 
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);
		}
		break;
	case WM_LBUTTONUP: // ������� �� ���������
		MessageBox(hWnd, "�� ��������!", "�������", MB_OK);
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

		glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(getShaderProgram());
		glBindVertexArray(getVArrayObject());
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		SwapBuffers(getHDC());

		
	}
}
