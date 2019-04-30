#pragma once

#include <windows.h>
#include <stdint.h>

class Camera;
class Shader;
class FontObject;

extern uint16_t					numberOfKeys;

extern WNDCLASSEX				pWndEngineClassEx;				// ��������� ������ ����

extern HDC						hDC;							// ���������� ����������
extern HGLRC					hRC;							// ����������

extern HWND						hWndEngine;						// ������� ���� ���������

extern HANDLE					debugConsole;

extern bool						isLoaded;						// �������� �� ���������

extern int64_t					mouseOffsetX,
								mouseOffsetY,
								lastMousePosX,
								lastMousePosY;

extern POINT					*mousePos;

extern RECT						*windowEngineRect;

extern bool						isCameraAction;

extern HINSTANCE				hInstance;

extern bool						*previousKeyboardState;

extern Shader					*ourShader,						// ������
								*fontShader,					// ������
								*selectShader;					// ������

extern FontObject				*font1;							// ����� ������ ������ �� �����

extern float					deltaTime;

extern Camera					*camera;

LRESULT WndEngineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void InitVarInterface();
void UninitVarInterface();

uint8_t InitWindow(HINSTANCE *hInstance);
void InitImgui();

bool isKeyDown(int key);
bool isKeyFirstPressed(int key);
bool isKeyFirstReleased(int key);

void CameraControllAction();

void Debug(unsigned int str);
void Debug(const char *sms);
void Debug(float str);
void Debug(int str);

void Clear();

void Loop();
