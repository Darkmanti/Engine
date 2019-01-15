#include <iostream>
#include <windows.h>
#include <stdint.h>

LRESULT CALLBACK WndProc(HWND, uint32_t, WPARAM, LPARAM);

//Указатель на структуру
WNDCLASSA* g_pWndClass;

ATOM RegisterEngineWindow(HINSTANCE hInstance)
{
	g_pWndClass->style = CS_VREDRAW | CS_HREDRAW;
	g_pWndClass->lpfnWndProc = WndProc;
	g_pWndClass->hInstance = hInstance;
	g_pWndClass->hCursor = LoadCursor(NULL, IDC_ARROW);
	g_pWndClass->hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
	g_pWndClass->lpszClassName = "EngineWndClass\0";
	
	return RegisterClass(g_pWndClass);
}

// Функция обработчика сообщений
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
	case WM_LBUTTONUP: // Реакция на сообщение
		MessageBox(hWnd, "Вы кликнули!", "Событие", MB_OK);
		break;
	  
    case WM_DESTROY: // Реакция на сообщение
		PostQuitMessage(0);
		break;
 
	default: // Все необработанные сообщения обработает сама Windows
		return DefWindowProc(hWnd, message, wParam, lParam);
  }
  
  return 0;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
{
	if (!RegisterEngineWindow(hInstance))
	{
		MessageBox(NULL, "Класс не зарегистрирован", "Ошибка", MB_OK);
		return 0;
	}
	
	HWND hWnd = CreateWindow(
		g_pWndClass->lpszClassName,
		static_cast<LPCTSTR>("Engine\0"),
		static_cast<DWORD>(g_pWndClass->style),
		0, 0, // Позиция
		200, 200, // Размер
		0,
		0,
		hInstance,
		0
	);
	
	if (hWnd == 0)
	{
		MessageBox(hWnd, "Ошибка", "Окно не создано", MB_OK);
		return 0;
	}
	
	MSG message {0}; 	// Структура сообщения к окну
	int8_t iResult;	// Код состояния
	
	// Пока есть сообщения
	while ((iResult = GetMessage(&message, NULL, 0, 0)))
	{
		// Если система вернула отрицательный код (ошибка), то выходим из цикла обработки
		if (iResult < 0) break;
		
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	
	return 0;
}