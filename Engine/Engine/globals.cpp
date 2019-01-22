#include "globals.h"

#include <string>

HINSTANCE		hInstance;				// ���������� ����������
WNDCLASSA		pWndClassA;				// ��������� ������ ����
HWND			hWnd;					// ���������� ����
HDC				hDC;					// ���������� ����������
HGLRC			hRC;					// ���������� ...

std::string		dirAppData;				// ���� ������������ ���������

GLuint			vArrayObject,			// VAO
				shaderProgram,			// ��������� ��������� ���������� � wndInit.cpp
	
				windowWidth,
				windowHeight,
	
				windowRenderWidth,		// ������ �������� �����. ���������� ������������ �������
				windowRenderHeight;		// ������ �������� �����. ���������� ������������ �������

// ���������� ����������
HINSTANCE &getHInstance()
{
	return hInstance;
}

// ��������� ������ ����
WNDCLASSA	&getWndClassA()
{
	return pWndClassA;
}

// ���������� ����
HWND &getHWnd()
{
	return hWnd;
}

// ���������� ����������
HDC &getHDC()
{
	return hDC;
}

// ���������� ...
HGLRC &getHRC()
{
	return hRC;
}

// dirAppData
std::string &getDirAppData()
{
	return dirAppData;
}

// VAO
GLuint &getVArrayObject()
{
	return vArrayObject;
}

// ��������� ���������
GLuint &getShaderProgram()
{
	return shaderProgram;
}

// ������ ����
GLuint &getWindowWidth()
{
	return windowWidth;
}

// ������ ����
GLuint &getWindowHeight()
{
	return windowHeight;
}

// ������ ������-����
GLuint &getWindowRenderWidth()
{
	return windowRenderWidth;
}

// ������ ������-����
GLuint &getWindowRenderHeight()
{
	return windowRenderHeight;
}