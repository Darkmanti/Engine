#include "WinApi.h"

#include <CommCtrl.h>
#include <algorithm>
#include <thread>
#include <windowsx.h>

#pragma comment(lib,"ComCtl32.Lib")

namespace WinApi
{
	WNDCLASSEX			pWndEngineClassEx,				// Структура класса окна
		pWndRenderClassEx;				// Структура класса рендер окна

// Различные дескрипторы
	HDC					hDC;							// Дескриптор устройства
	HGLRC				hRC;							// Дескпритор ...

	HWND				hWndEngine,						// Главное окно редактора
		hWndRender,						// Окно рендера внутри редактора

		hWndListViewLocation,			// ListView локации
		hWndListViewProject;			// ListView проекта

		// Меню
	HMENU				hMenu,
		hPopMenuFile,
		hPopMenuScene,
		hPopMenuProject,
		hPopMenuProjectImport;

	OPENFILENAME		OFN{ 0 };

	char				szDirect[MAX_PATH],
		szFileName[MAX_PATH];

	bool				isLoaded;

	HINSTANCE			hInstance;

	extern const uint16_t NumberOfKeys = 256;

	bool previousKeyboardState[NumberOfKeys];

	bool ListViewAddItem(const char* elementName, HWND hWndListView)
	{
		LVITEM lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iSubItem = 0;
		lvi.iItem = 0x7FFFFFFF;
		lvi.pszText = (LPSTR)elementName;

		// Добавить элемент
		if (ListView_InsertItem(hWndListView, &lvi) < 0)
		{
			return true;
		}

		return false;
	}

	bool ListViewRemoveItem(HWND hWndListView, int16_t index)
	{
		if (index >= 0)
		{
			ListView_DeleteItem(hWndListView, index);
		}

		return false;
	}

	// Регистрация класса окна
	ATOM RegisterWindowEngine()
	{
		// Описываем поля структур
		pWndEngineClassEx.cbSize = sizeof(WNDCLASSEX);								// Размер в байтах структуры класса
		pWndEngineClassEx.style = CS_VREDRAW | CS_HREDRAW;							// Стиль окна
		pWndEngineClassEx.lpfnWndProc = WndEngineProc;								// Указатель на оконную процедуру
		pWndEngineClassEx.hInstance = hInstance;							// Дескриптор приложения
		pWndEngineClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);					// Подгружам курсор
		pWndEngineClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);				// Указатель на кисть с цветом фона (Типо кисть - рисование)
		pWndEngineClassEx.lpszClassName = "WndEngineClass";							// Наименование класса
		pWndEngineClassEx.hIcon = LoadIcon(hInstance, "IDI_ENGINEICON");	// Иконка

		if (int16_t iError = RegisterClassEx(&pWndEngineClassEx))
		{

		}
		else
		{
			MessageBox(NULL, "Класс не зарегистрирован", "RegisterWindowEngine", MB_OK);
			return iError;
		}

		return 0;
	}
	ATOM RegisterWindowRender()
	{
		pWndRenderClassEx.cbSize = sizeof(WNDCLASSEX);						// Размер в байтах структуры класса
		pWndRenderClassEx.style = CS_VREDRAW | CS_HREDRAW | CS_BYTEALIGNCLIENT;					// Стиль окна
		pWndRenderClassEx.lpfnWndProc = WndRenderProc;						// Указатель на оконную процедуру
		pWndRenderClassEx.hInstance = hInstance;					// Дескриптор приложения
		pWndRenderClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);			// Подгружам курсор
		pWndRenderClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);		// Указатель на кисть с цветом фона (Типо кисть - рисование)
		pWndRenderClassEx.lpszClassName = "WndRenderClass";					// Наименование класса

		if (int16_t iError = RegisterClassEx(&pWndRenderClassEx))
		{

		}
		else
		{
			MessageBox(NULL, "Класс не зарегистрирован", "RegisterWindowRender", MB_OK);
			return iError;
		}

		return 0;
	}

	// Создание окна
	uint16_t CreateWindowEngine()
	{
		// Дескриптор окна
		hWndEngine = CreateWindowEx(WS_EX_ACCEPTFILES,									// Extended style
			pWndEngineClassEx.lpszClassName,											// Название класса
			"Движок",																	// Название окна
			WS_OVERLAPPEDWINDOW,														// Стиль окна
			0, 0,											// Позиция
			1366, 768,													// Размер
			0,																			// Родительское окно
			0,																			// Меню
			hInstance,															// Десприптор приложения
			0																			// Все это говно доступно на msdn
		);

		// Проверка на создание окна
		if (!hWndEngine)
		{
			MessageBox(NULL, "Окно не создано", "Ошибка", MB_OK);
			return 1;
		}

		return 0;
	}
	uint16_t CreateWindowRender()
	{
		hWndRender = CreateWindowEx(WS_EX_ACCEPTFILES,		// Extended style
			pWndRenderClassEx.lpszClassName,				// Название класса
			"Рендер",										// Название окна
			WS_THICKFRAME | WS_CHILD,						// Стиль окна
			256, 256,	// Позиция
			800, 600,			// Размер
			hWndEngine,										// Родительское окно
			0,												// Меню
			hInstance,								// Десприптор приложения
			0												// Все это говно доступно на msdn
		);

		// Проверка на создание окна
		if (!hWndRender)
		{
			MessageBox(NULL, "Окно не создано", "Ошибка", MB_OK);
			return 1;
		}

		return 0;
	}
	uint16_t CreateListViewLocation()
	{
		hWndListViewLocation = CreateWindowEx(NULL,
			WC_LISTVIEW,
			"Сцена",
			WS_VISIBLE | LVS_SINGLESEL | LVS_REPORT | WS_CHILD,
			0, 0,
			192, 256,
			hWndEngine,
			NULL,
			hInstance,
			NULL);

		if (!hWndListViewLocation)
		{
			return 1;
		}

		// Дополнительные свойства
		SetWindowSubclass(hWndListViewLocation, SubClassLocationProc, 1, NULL);
		ListView_SetExtendedListViewStyle(hWndListViewLocation, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		// Добавить столбцы
		LVCOLUMN lvc;
		lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.iSubItem = 0;
		lvc.pszText = (LPSTR)"Имя";
		lvc.cx = 100;

		if (ListView_InsertColumn(hWndListViewLocation, 0, &lvc) == -1)
		{
			return 1;
		}

		return 0;
	}
	uint16_t CreateListViewProject()
	{
		hWndListViewProject = CreateWindowEx(NULL,
			WC_LISTVIEW,
			"Проект",
			WS_VISIBLE | LVS_SINGLESEL | LVS_REPORT | WS_CHILD,
			0, 256,
			192, 256,
			hWndEngine,
			NULL,
			hInstance,
			NULL);

		if (!hWndListViewProject)
		{
			return 1;
		}

		SetWindowSubclass(hWndListViewProject, SubClassProjectProc, 2, NULL);
		ListView_SetExtendedListViewStyle(hWndListViewProject, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		// Добавить столбцы
		LVCOLUMN lvc;
		lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.iSubItem = 0;
		lvc.pszText = (LPSTR)"Имя";
		lvc.cx = 100;

		if (ListView_InsertColumn(hWndListViewProject, 0, &lvc) == -1)
		{
			return 1;
		}

		lvc.pszText = (LPSTR)"Тип";
		lvc.iSubItem = 1;
		if (ListView_InsertColumn(hWndListViewProject, 0, &lvc) == -1)
		{
			return 1;
		}

		return 0;
	}
	uint16_t CreateMainMenu()
	{
		hMenu = CreateMenu();
		hPopMenuFile = CreatePopupMenu();
		hPopMenuScene = CreatePopupMenu();
		hPopMenuProject = CreatePopupMenu();
		hPopMenuProjectImport = CreatePopupMenu();

		if (!hMenu || !hPopMenuFile || !hPopMenuProject || !hPopMenuProjectImport)
		{
			return 1;
		}

		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuFile, "&Файл");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_NEWLOCATION, "Новая локация\tCtrl+N");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_OPENLOCATION, "Открыть локацию\tCtrl+O");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_SAVELOCATION, "Сохранить\tCtrl+S");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_SAVEASLOCATION, "Сохранить как\tCtrl+Shift+S");
		AppendMenu(hPopMenuFile, MF_SEPARATOR, MENU_FILE_SAVEASLOCATION + 1, "");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_QUIT, "Выход\tAlt+F4");

		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuScene, "&Сцена");
		AppendMenu(hPopMenuScene, MF_STRING, MENU_SCENE_CREATE, "Создать");
		AppendMenu(hPopMenuScene, MF_STRING, MENU_SCENE_DELETE, "Удалить");

		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuProject, "&Проект");
		AppendMenu(hPopMenuProject, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuProjectImport, "&Импорт");
		AppendMenu(hPopMenuProjectImport, MF_STRING, MENU_PROJECT_IMPORT_MODEL, "Модель");
		AppendMenu(hPopMenuProjectImport, MF_STRING, MENU_PROJECT_IMPORT_TEXTURE, "Текстуру");

		SetMenu(hWndEngine, hMenu);

		return 0;
	}

	// Инициализация интерфейса
	uint16_t InitInterface()
	{
		uint16_t iError;

		if (iError = RegisterWindowEngine()) return iError * 10 + 1;
		if (iError = RegisterWindowRender()) return iError * 10 + 2;

		// Проверка на создание окна
		if (iError = CreateWindowEngine()) return iError * 10 + 5;
		if (iError = CreateWindowRender()) return iError * 10 + 6;

		if (iError = CreateListViewLocation()) return iError * 10 + 9;
		if (iError = CreateListViewProject()) return iError * 10 + 10;

		if (iError = CreateMainMenu()) return iError * 10 + 12;

		return 0;
	}

	uint16_t ShowInterface(const int16_t nCmdShow)
	{
		if (ShowWindow(hWndEngine, nCmdShow)) return 2;
		if (!UpdateWindow(hWndEngine)) return 3;

		if (ShowWindow(hWndRender, SW_SHOWNORMAL)) return 4;
		if (!UpdateWindow(hWndRender)) return 5;

		return 0;
	}

	// Оконные процедуры
	LRESULT WndEngineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (!isLoaded)
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		switch (message)
		{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case MENU_FILE_NEWLOCATION:
				//
				break;

			case MENU_FILE_OPENLOCATION:
				OFN.lStructSize = sizeof(OFN);
				OFN.hwndOwner = NULL;
				OFN.lpstrFile = szDirect;
				*(OFN.lpstrFile) = 0;
				OFN.nMaxFile = sizeof(szDirect);
				OFN.lpstrFilter = NULL;
				OFN.nFilterIndex = 1;
				OFN.lpstrFileTitle = szFileName;
				*(OFN.lpstrFileTitle) = 0;
				OFN.nMaxFileTitle = sizeof(szFileName);
				OFN.lpstrInitialDir = NULL;
				OFN.Flags = OFN_EXPLORER;
				GetOpenFileName(&OFN);
				break;

			case MENU_FILE_SAVELOCATION:
				break;

			case MENU_FILE_SAVEASLOCATION:
				// Диалог сохранения
				OFN.lStructSize = sizeof(OFN);
				OFN.hwndOwner = NULL;
				OFN.lpstrFile = szDirect;
				*(OFN.lpstrFile) = 0;
				OFN.nMaxFile = sizeof(szDirect);
				OFN.lpstrFilter = NULL;
				OFN.nFilterIndex = 1;
				OFN.lpstrFileTitle = szFileName;
				*(OFN.lpstrFileTitle) = 0;
				OFN.nMaxFileTitle = sizeof(szFileName);
				OFN.lpstrInitialDir = NULL;
				OFN.Flags = OFN_EXPLORER;
				GetSaveFileName(&OFN);
				break;

			case MENU_FILE_QUIT:
				// Выход
				PostQuitMessage(0);
				break;

			case MENU_SCENE_CREATE:
				ListViewAddItem("GameObject", hWndListViewLocation);
				break;

			case MENU_SCENE_DELETE:
				ListViewRemoveItem(hWndListViewLocation, 0);
				break;

			case MENU_PROJECT_IMPORT_MODEL:
				//
				break;
			case MENU_PROJECT_IMPORT_TEXTURE:
				//
				break;
			}

			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	LRESULT WndRenderProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (!isLoaded)
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		switch (message)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	LRESULT CALLBACK SubClassLocationProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		switch (message)
		{
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, SubClassLocationProc, uIdSubclass);
			break;

		default:
			DefSubclassProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	LRESULT CALLBACK SubClassProjectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{

		switch (message)
		{
		case WM_NCDESTROY:
			RemoveWindowSubclass(hWnd, SubClassProjectProc, uIdSubclass);
			break;
		default:
			DefSubclassProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	bool isKeyDown(int key)
	{
		return (GetAsyncKeyState(key) & (1 << 16));
	}

	bool isKeyFirstPressed(int key)
	{
		bool previousState = previousKeyboardState[key];

		previousKeyboardState[key] = isKeyDown(key);

		return (previousKeyboardState[key] && !previousState);
	}

	bool isKeyFirstReleased(int key)
	{
		bool previousState = previousKeyboardState[key];

		previousKeyboardState[key] = isKeyDown(key);

		return (!previousKeyboardState[key] && previousState);
	}

	// Метод с циклом программы
	void Loop()
	{
		MSG message{ 0 }; 	// Структура сообщения к окну
		int8_t iResult;		// Код состояния

		// Пока есть сообщения
		// Если система вернула отрицательный код (ошибка), то выходим из цикла обработки
		while ((iResult = GetMessage(&message, NULL, 0, 0)))
		{
			// Обрабатываем сообщения в WndProc
			TranslateMessage(&message);
			DispatchMessage(&message);

			if (isKeyDown(VK_A))
			{
				ListViewAddItem("dwad", hWndListViewLocation);
				ListViewAddItem("32323", hWndListViewProject);
			}


		}
	}

	void InitInput()
	{
		for (uint16_t keyNum = 0; keyNum < NumberOfKeys; ++keyNum)
		{
			previousKeyboardState[keyNum] = isKeyDown(keyNum);
		}
	}
};
