#pragma once

#include "GLEW/glew.h"

#include <windows.h>
#include <stdint.h>

// ��� ��� �������� � ����� � ������� winapi
namespace Output
{
	extern uint32_t			windowWidth;			// ������ ���� ���������
	extern uint32_t			windowHeight;			// ������ ���� ���������

	extern uint32_t			windowRenderWidth;		// ������ ���� �������. ���������� ������������ �������
	extern uint32_t			windowRenderHeight;		// ������ ���� �������. ���������� ������������ �������


	extern WNDCLASSEX		pWndClassEx;			// ��������� ������ ����


	extern GLuint			vArrayObject;			// VAO
	extern GLuint			vBufferObject;			// VBO
	extern GLuint			shaderProgram;			// ��������� ���������


	// ��������� �����������
	extern HDC			hDC;						// ���������� ����������
	extern HGLRC		hRC;						// ���������� ...

	extern HWND hWnd;								// ������� ���� ���������
	extern HWND hWindowRender;						// ���� ������� ������ ���������

	// ����������� ����
	ATOM RegisterEngineWindow(const HINSTANCE hInstance);

	// �������� ����
	uint8_t CreateEngineWindow(const int32_t nCmdShow);

	// ������� ����������� ��������� (��������� ����)
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// ����� � ������ ���������
	void Loop();

	// ������������� ����������
	int16_t InitInterface(const int32_t);

	void EnableOpenGL();
	void DisableOpenGL();
};
