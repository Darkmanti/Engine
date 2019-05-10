#pragma once

#include <windows.h>
#include <stdint.h>

class Camera;
class Shader;
class FontObject;

extern uint16_t					numberOfKeys;

extern WNDCLASSEX				pWndEngineClassEx;				// Структура класса окна

extern HDC						hDC;							// Дескриптор устройства
extern HGLRC					hRC;							// Дескпритор

extern HWND						hWndEngine;						// Главное окно редактора

extern HANDLE					debugConsole;

extern bool						isLoaded;						// Загружен ли интерфейс

extern int64_t					mouseOffsetX,
								mouseOffsetY,
								lastMousePosX,
								lastMousePosY;

extern POINT					*mousePos;

extern bool						isCameraAction;

extern HINSTANCE				hInstance;

extern bool						*previousKeyboardState;

extern Shader					*ourShader,						// Шейдер
								*fontShader,					// Шейдер
								*selectShader;					// Шейдер

extern FontObject				*font1;							// Шрифт вывода текста на экран

extern float					deltaTime;

extern Camera					*camera;

void InitVarInterface();
void UninitVarInterface();

uint8_t InitWindow(HINSTANCE *hInstance);
void InitImgui();

bool isKeyDown(int key);
bool isKeyFirstPressed(int key);
bool isKeyFirstReleased(int key);

void CameraControllAction();

void Clear();

void Loop();
