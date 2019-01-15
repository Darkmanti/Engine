#include "windows.h"
#include "stdint.h"

// Пошли предварительные объявления
// Объявляем глобальную переменную
extern HINSTANCE hInstance;

// Объявили будущий метод для последующей передачи его указателя в структуру WNDCLASSA
LRESULT WndProc(HWND, uint32_t, WPARAM, LPARAM);

// Объявили метод регистрации окна для семантической группировки стейтментов
ATOM RegisterEngineWindow(const HINSTANCE);

// Объявили создание окна
uint8_t CreateEngineWindow(const int32_t);

// Основной цикл программы
void Loop();

// Точка входа в программу
int WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int32_t nCmdShow)
{
	// Отправляем дескриптор в файл wndInit
	hInstance = hInst;

	if (!RegisterEngineWindow(hInst)) return 0; // Проверка на валидность регистрации окна
	if (!CreateEngineWindow(nCmdShow)) return 0; // Проверка на валидность регистрации окна

	Loop();
	
	return 0;
}
