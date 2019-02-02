#pragma once

#include "GLEW/glew.h"

#include <windows.h>
#include <stdint.h>

// ��� ��� �������� � ����� � ������� winapi
namespace Output
{
	extern GLuint			windowWidth;			// ������ ���� ���������
	extern GLuint			windowHeight;			// ������ ���� ���������

	extern GLuint			windowRenderWidth;		// ������ ���� �������. ���������� ������������ �������
	extern GLuint			windowRenderHeight;		// ������ ���� �������. ���������� ������������ �������


	extern WNDCLASSEX		pWndClassEx;			// ��������� ������ ����


	extern GLuint			vArrayObject;			// VAO
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
	int16_t InitInterface();

	void EnableOpenGL();
	void DisableOpenGL();
};
