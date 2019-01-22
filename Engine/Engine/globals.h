#pragma once

#ifndef ENGINE_GLOBALS

#include "GLEW/glew.h"

#include <windows.h>
#include <string>

#endif

HINSTANCE		&getHInstance();					// ���������� ����������
WNDCLASSA		&getWndClassA();					// ��������� ������ ����
HWND			&getHWnd();							// ���������� ����
HDC				&getHDC();							// ���������� ����������
HGLRC			&getHRC();							// ���������� ...
std::string		&getDirAppData();					// dirAppData
GLuint			&getVArrayObject();					// VAO
GLuint			&getShaderProgram();				// ��������� ���������
GLuint			&getWindowWidth();					// ������ ����
GLuint			&getWindowHeight();					// ������ ����
GLuint			&getWindowRenderWidth();			// ������ ������-�����
GLuint			&getWindowRenderHeight();			// ������ ������-�����
