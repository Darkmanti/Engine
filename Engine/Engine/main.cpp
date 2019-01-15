#include <iostream>
#include <windows.h>

typedef  struct
{
	UINT style; 			// Стиль окна
	WNDPROC lpfnWndProc 	// Указатель на оконную процедуру
	int32_t cbClsExtra; 	// Дополнительные байты после класса
	int32_t cbWndExtra; 	// Дополнительные байты после экземпляра окна
	HINSTANCE hInstance; 	// Дескриптор окна. (Потом передаем его как параметр в WinMain)
	HICON hIcon; 			// Дескриптор иконки приложения
	HCURSOR hCursor; 		// Курсор приложения
	HBRUSH hbrBackground; 	// Фоновый цвет
	LPCTSTR lpszMenuName; 	// Наименование меню
	LPCTSTR lpszClassName; 	// Наименование класса окна
} WNDCLASS, *PWNDcLASS;



int WinMain()
{
	
	return 0;
}