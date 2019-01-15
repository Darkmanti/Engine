// Файл содержит в себе члены блока инициализации главного окна
#include "windows.h"
#include "stdint.h"

// Объявили будущий метод для последующей передачи его указателя в структуру WNDCLASSA
LRESULT WndProc(HWND, uint32_t, WPARAM, LPARAM);

// Структура класса окна
WNDCLASSA g_pWndClassA;

// Дескриптор приложения
HINSTANCE hInstance;

// Дескриптор окна
HWND hWnd;

// Регистрация окна для семантической группировки стейтментов
ATOM RegisterEngineWindow(const HINSTANCE hInstance)
{
	// Описываем поля структур
	g_pWndClassA.style = CS_VREDRAW | CS_HREDRAW;			// Стиль окна
	g_pWndClassA.lpfnWndProc = WndProc;						// Указатель на оконную процедуру
	g_pWndClassA.hInstance = hInstance;						// Дескриптор приложения
	g_pWndClassA.hCursor = LoadCursor(NULL, IDC_ARROW);		// Подгружам курсор
	g_pWndClassA.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);	// Указатель на кисть с цветом фона (Типо кисть - рисование)
	g_pWndClassA.lpszClassName = "EngineWndClass\0";		// Наименование класса

	int16_t iError(RegisterClass(&g_pWndClassA));

	// Проверка на валидность регистрации окна
	if (!iError)
	{
		MessageBox(NULL, "Класс не зарегистрирован", "Ошибка", MB_OK);
		return 0;
	}

	return iError;
}

// Создание окна
uint8_t CreateEngineWindow(const int32_t nCmdShow)
{
	// Дескриптор окна
	hWnd = CreateWindow(
		g_pWndClassA.lpszClassName,				// Название класса
		static_cast<LPCTSTR>("Engine\0"),		// Название окна
		static_cast<DWORD>(g_pWndClassA.style), // Стиль окна
		0, 0,									// Позиция
		640, 480,								// Размер
		0,										// Еще какая-то залупа
		0,										// И еще что-то там
		hInstance,								// Десприптор приложения
		0										// Все это говно доступно на msdn
	);

	if (!hWnd)
	{
		MessageBox(hWnd, "Ошибка", "Окно не создано", MB_OK);
		return 0;
	}

	// Показать окно
	ShowWindow(hWnd, nCmdShow);

	// Обновлять окно
	UpdateWindow(hWnd);

	return 1;
}

// Функция обработчика сообщений (Процедура окна)
LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

// Метод с циклом программы
void Loop()
{
	MSG message{ 0 }; 	// Структура сообщения к окну
	int8_t iResult;		// Код состояния

	// Пока есть сообщения
	while ((iResult = GetMessage(&message, NULL, 0, 0)))
	{
		// Если система вернула отрицательный код (ошибка), то выходим из цикла обработки
		if (iResult < 0) break;

		// Иначе обрабатываем сообщения в WndProc
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}
