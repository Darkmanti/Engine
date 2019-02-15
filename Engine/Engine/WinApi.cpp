#include "WinApi.h"

#include <CommCtrl.h>
#include <algorithm>

namespace WinApi
{
	int32_t				windowPositionX,				// Позиция окна по X
						windowPositionY,				// Позиция окна по Y
						windowWidth,					// Ширина окна редактора
						windowHeight,					// Высота окна редактора

						windowRenderPositionX,			// Позиция окна рендера по X
						windowRenderPositionY,			// Позиция окна рендера по Y
						windowRenderWidth,				// Ширина окна рендера. Изменяется относительно размера
						windowRenderHeight,				// Высота окна рендера. Изменяется относительно размера

						windowLocationPositionX,		// Позиция окна location по X
						windowLocationPositionY,		// Позиция окна location по Y
						windowLocationWidth,			// Ширина окна location
						windowLocationHeight,			// Высота окна location

						windowProjectPositionX,			// Позиция окна location по X
						windowProjectPositionY,			// Позиция окна location по Y
						windowProjectWidth,				// Ширина окна location
						windowProjectHeight,			// Высота окна location

						isFullscreen,					// Развернутое окно или нет
						buttonsSize;					// Размер кнопок на панели инструментов


	WNDCLASSEX			pWndEngineClassEx,				// Структура класса окна
						pWndRenderClassEx,				// Структура класса рендер окна
						pWndLocationClassEx,			// Структура класса location
						pWndProjectClassEx;				// Структура класса location


	GLuint				vbo, vao, ebo;


	// Различные дескрипторы
	HDC					hDC;							// Дескриптор устройства
	HGLRC				hRC;							// Дескпритор ...

	HWND				hWndEngine,						// Главное окно редактора
						hWndRender,						// Окно рендера внутри редактора
						hWndLocation,					// Окно локации
						hWndProject,					// Окно проекта

						hWndListViewLocation,			// ListView локации
						hWndListViewProject,			// ListView проекта

						hStatusWindow,

						// Кнопки
						hBtn_Shader,
						hBtn_OpenModel,
						hBtn_ShowModel,
						hBtn_CloseModel;

						// Меню
	HMENU				hMenu,
						hPopMenuFile,
						hPopMenuProject,
						hPopMenuProjectImport;

	OPENFILENAME		ofn{ 0 };

	uint8_t				setValue,
						*keys = new uint8_t[32];

	char				szDirect[MAX_PATH],
						szFileName[MAX_PATH];

	constexpr uint8_t option1 = 0x01;
	constexpr uint8_t option2 = 0x02;
	constexpr uint8_t option3 = 0x04;
	constexpr uint8_t option4 = 0x08;
	constexpr uint8_t option5 = 0x10;
	constexpr uint8_t option6 = 0x20;
	constexpr uint8_t option7 = 0x40;
	constexpr uint8_t option8 = 0x80;
	
	uint8_t getMask(WPARAM wParam)
	{
		switch ((uint32_t)wParam % 8 + 1)
		{
		case option1:
			return option1;
		case option2:
			return option2;
		case option3:
			return option3;
		case option4:
			return option4;
		case option5:
			return option5;
		case option6:
			return option6;
		case option7:
			return option7;
		case option8:
			return option8;
		}
	}

	bool LocationAddItem(const char* elementName)
	{
		LVITEM lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iSubItem = 0;
		lvi.iItem = 0x7FFFFFFF;
		lvi.pszText = (LPSTR)elementName;
		
		// Добавить элемент
		if (ListView_InsertItem(hWndListViewLocation, &lvi) < 0)
		{
			return true;
		}

		return false;
	}

	// Регистрация класса окна
	void BindKey(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_KEYDOWN:
			setValue = getMask(wParam);
			keys[(uint32_t)wParam / 8] |= setValue;

			switch (wParam)
			{
			case VK_F1:
				LocationAddItem("F1");

				return;
			default:
				return;
			}

		case WM_KEYUP:
			setValue = getMask(wParam);
			keys[(uint32_t)wParam / 8] &= ~setValue;

			switch (wParam)
			{
			case VK_F1:
				LocationAddItem("F1");

				return;
			default:
				return;
			}
		}
	}

	// Регистрация класса окна
	ATOM RegisterWindowEngine()
	{
		// Описываем поля структур
		pWndEngineClassEx.cbSize = sizeof(WNDCLASSEX);								// Размер в байтах структуры класса
		pWndEngineClassEx.style = CS_VREDRAW | CS_HREDRAW;							// Стиль окна
		pWndEngineClassEx.lpfnWndProc = WndEngineProc;								// Указатель на оконную процедуру
		pWndEngineClassEx.hInstance = Engine::hInstance;							// Дескриптор приложения
		pWndEngineClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);					// Подгружам курсор
		pWndEngineClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);				// Указатель на кисть с цветом фона (Типо кисть - рисование)
		pWndEngineClassEx.lpszClassName = "WndEngineClass";							// Наименование класса
		pWndEngineClassEx.hIcon = LoadIcon(Engine::hInstance, "IDI_ENGINEICON");	// Иконка

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
		pWndRenderClassEx.hInstance = Engine::hInstance;					// Дескриптор приложения
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
	ATOM RegisterWindowLocation()
	{
		pWndLocationClassEx.cbSize = sizeof(WNDCLASSEX);							// Размер в байтах структуры класса
		pWndLocationClassEx.style = CS_VREDRAW | CS_HREDRAW;						// Стиль окна
		pWndLocationClassEx.lpfnWndProc = WndLocationProc;							// Указатель на оконную процедуру
		pWndLocationClassEx.hInstance = Engine::hInstance;							// Дескриптор приложения
		pWndLocationClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);					// Подгружам курсор
		pWndLocationClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);			// Указатель на кисть с цветом фона (Типо кисть - рисование)
		pWndLocationClassEx.lpszClassName = "WndLocationClass";						// Наименование класса

		if (int16_t iError = RegisterClassEx(&pWndLocationClassEx))
		{

		}
		else
		{
			MessageBox(NULL, "Класс не зарегистрирован", "RegisterWindowLocation", MB_OK);
			return iError;
		}

		return 0;
	}
	ATOM RegisterWindowProject()
	{
		pWndProjectClassEx.cbSize = sizeof(WNDCLASSEX);								// Размер в байтах структуры класса
		pWndProjectClassEx.style = CS_VREDRAW | CS_HREDRAW;							// Стиль окна
		pWndProjectClassEx.lpfnWndProc = WndProjectProc;							// Указатель на оконную процедуру
		pWndProjectClassEx.hInstance = Engine::hInstance;							// Дескриптор приложения
		pWndProjectClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);					// Подгружам курсор
		pWndProjectClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);				// Указатель на кисть с цветом фона (Типо кисть - рисование)
		pWndProjectClassEx.lpszClassName = "WndProjectClass";						// Наименование класса

		if (int16_t iError = RegisterClassEx(&pWndProjectClassEx))
		{

		}
		else
		{
			MessageBox(NULL, "Класс не зарегистрирован", "RegisterWindowProject", MB_OK);
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
			windowPositionX, windowPositionY,											// Позиция
			windowWidth, windowHeight,													// Размер
			0,																			// Родительское окно
			0,																			// Меню
			Engine::hInstance,															// Десприптор приложения
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
		hWndRender = CreateWindowEx(WS_EX_ACCEPTFILES,					// Extended style
			pWndRenderClassEx.lpszClassName,				// Название класса
			"Рендер",										// Название окна
			WS_THICKFRAME | WS_CHILD,						// Стиль окна
			windowRenderPositionX, windowRenderPositionY,	// Позиция
			windowRenderWidth, windowRenderHeight,			// Размер
			hWndEngine,										// Родительское окно
			0,												// Меню
			Engine::hInstance,								// Десприптор приложения
			0												// Все это говно доступно на msdn
		);

		// Проверка на создание окна
		if (!hWndRender)
		{
			MessageBox(NULL, "Окно не создано", "Ошибка", MB_OK);
			return 1;
		}

		Engine::camera = new Camera(glm::vec3(0, 4, 100));

		return 0;
	}
	uint16_t CreateWindowLocation()
	{
		hWndLocation = CreateWindowEx(NULL,
			pWndLocationClassEx.lpszClassName,
			"Локация",
			WS_THICKFRAME | WS_CAPTION,
			windowLocationPositionX, windowLocationPositionY,
			windowLocationWidth, windowLocationHeight,
			hWndEngine,
			0,
			Engine::hInstance,
			0);

		if (!hWndLocation)
		{
			return 1;
		}

		return 0;
	}
	uint16_t CreateWindowProject()
	{
		hWndProject = CreateWindowEx(NULL,
			pWndProjectClassEx.lpszClassName,
			"Проект",
			WS_THICKFRAME | WS_CAPTION,
			windowProjectPositionX, windowProjectPositionY,
			windowProjectWidth, windowProjectHeight,
			hWndEngine,
			0,
			Engine::hInstance,
			0);

		if (!hWndProject)
		{
			return 1;
		}

		return 0;
	}
	uint16_t CreateListViewLocation()
	{
		hWndListViewLocation = CreateWindowEx(WS_EX_ACCEPTFILES,
			WC_LISTVIEW,
			"",
			WS_VISIBLE | WS_CHILD | LVS_SINGLESEL | LVS_REPORT,
			0, 0,
			windowLocationWidth, windowLocationHeight,
			hWndLocation,
			NULL,
			Engine::hInstance,
			NULL);

		if (!hWndListViewLocation)
		{
			return 1;
		}

		// Дополнительные свойства
		ListView_SetExtendedListViewStyleEx(hWndListViewLocation, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

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
		hWndListViewProject = CreateWindowEx(WS_EX_ACCEPTFILES,
			WC_LISTVIEW,
			"",
			WS_VISIBLE | WS_CHILD,
			0, 0,
			windowProjectWidth, windowProjectHeight,
			hWndProject,
			NULL,
			Engine::hInstance,
			NULL);

		if (!hWndListViewProject)
		{
			return 1;
		}

		LVCOLUMN lvc;
		int iCol(0);

		lvc.iSubItem = iCol;
		lvc.pszText = (LPSTR)"dfsdf";
		lvc.cx = 100;               // Width of column in pixels.

		if (iCol < 2)
			lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
		else
			lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.

		// Insert the columns into the list view.
		if (ListView_InsertColumn(hWndListViewProject, iCol, &lvc) == -1)
			return FALSE;

		return 0;
	}
	uint16_t CreateMainMenu()
	{
		hMenu = CreateMenu();
		hPopMenuFile = CreatePopupMenu();
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

		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuProject, "&Проект");
		AppendMenu(hPopMenuProject, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuProjectImport, "&Импорт");
		AppendMenu(hPopMenuProjectImport, MF_STRING, MENU_PROJECT_IMPORT_MODEL, "Модель");
		AppendMenu(hPopMenuProjectImport, MF_STRING, MENU_PROJECT_IMPORT_TEXTURE, "Текстуру");

		SetMenu(hWndEngine, hMenu);

		return 0;
	}
	// Создание кнопок управления
	uint16_t CreateButtons()
	{
		// Кнопка компиляции шейдерной программы
		hBtn_Shader = CreateWindow("BUTTON",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			0, 0,
			buttonsSize, buttonsSize,
			hWndEngine,
			0,
			Engine::hInstance,
			NULL);

		if (!hBtn_Shader)
		{
			return 1;
		}

		// Кнопка загрузки модели
		hBtn_OpenModel = CreateWindow("BUTTON",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			buttonsSize, 0,
			buttonsSize, buttonsSize,
			hWndEngine,
			0,
			Engine::hInstance,
			NULL);

		if (!hBtn_OpenModel)
		{
			return 2;
		}

		// Кнопка отрисовки модели
		HWND &hBtn();
		hBtn_ShowModel = CreateWindow("BUTTON",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			buttonsSize * 2, 0,
			buttonsSize, buttonsSize,
			hWndEngine,
			0,
			Engine::hInstance,
			NULL);

		if (!hBtn_ShowModel)
		{
			return 3;
		}

		// Кнопка выгрузки модели
		hBtn_CloseModel = CreateWindow("BUTTON",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			buttonsSize * 3, 0,
			buttonsSize, buttonsSize,
			hWndEngine,
			0,
			Engine::hInstance,
			NULL);

		if (!hBtn_CloseModel)
		{
			return 4;
		}

		return 0;
	}

	uint16_t AddListViewLocation(const char* pCol1)
	{
		LVITEM lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iSubItem = 0;
		lvi.iItem = 0x7FFFFFFF;
		lvi.pszText = (LPSTR)pCol1;

		// Добавить элемент
		int index = ListView_InsertItem(hWndListViewLocation, &lvi);

		if (index < 0)
		{
			return 1;
		}

		return 0;
	}

	// Инициализация интерфейса
	uint16_t InitInterface()
	{
		uint16_t iError;

		if (iError = RegisterWindowEngine()) return iError * 10 + 1;
		if (iError = RegisterWindowRender()) return iError * 10 + 2;
		if (iError = RegisterWindowLocation()) return iError * 10 + 3;
		if (iError = RegisterWindowProject()) return iError * 10 + 4;

		// Проверка на создание окна
		if (iError = CreateWindowEngine()) return iError * 10 + 5;
		if (iError = CreateWindowRender()) return iError * 10 + 6;

		if (iError = CreateWindowLocation()) return iError * 10 + 7;
		if (iError = CreateWindowProject()) return iError * 10 + 8;

		if (iError = CreateListViewLocation()) return iError * 10 + 9;
		if (iError = CreateListViewProject()) return iError * 10 + 10;

		if (iError = CreateButtons()) return iError * 10 + 11;
		if (iError = CreateMainMenu()) return iError * 10 + 12;

		return 0;
	}

	uint16_t ShowInterface(const int16_t nCmdShow)
	{
		if (WinApi::isFullscreen)
		{
			if (ShowWindow(hWndEngine, SW_MAXIMIZE)) return 1;
		}
		else
		{
			if (ShowWindow(hWndEngine, nCmdShow)) return 2;
		}

		if (!UpdateWindow(hWndEngine)) return 3;

		if (ShowWindow(hWndRender, SW_SHOWNORMAL)) return 4;
		if (!UpdateWindow(hWndRender)) return 5;

		if (ShowWindow(hWndLocation, SW_SHOWNORMAL)) return 6;
		if (!UpdateWindow(hWndLocation)) return 7;

		if (ShowWindow(hWndProject, SW_SHOWNORMAL)) return 8;
		if (!UpdateWindow(hWndProject)) return 9;

		// Показываем кнопки
		if (!ShowWindow(hBtn_OpenModel, SW_SHOWNORMAL)) return 12;
		if (!ShowWindow(hBtn_ShowModel, SW_SHOWNORMAL)) return 13;
		if (!ShowWindow(hBtn_Shader, SW_SHOWNORMAL)) return 14;
		if (!ShowWindow(hBtn_CloseModel, SW_SHOWNORMAL)) return 15;

		return 0;
	}

	//constexpr uint8_t option1 = 0x01;
	//constexpr uint8_t option2 = 0x02;
	//constexpr uint8_t option3 = 0x04;
	//constexpr uint8_t option4 = 0x08;
	//constexpr uint8_t option5 = 0x10;
	//constexpr uint8_t option6 = 0x20;
	//constexpr uint8_t option7 = 0x40;
	//constexpr uint8_t option8 = 0x80;

	//uint8_t *keys = new uint8_t[32];

	// Оконные процедуры
	LRESULT WndEngineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (!Engine::isLoaded)
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		BindKey(hWnd, message, wParam, lParam);

		switch (message)
		{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case MENU_FILE_NEWLOCATION:
				//
				break;
			case MENU_FILE_OPENLOCATION:
				// Открытие диалога сохранения файла локации

				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = NULL;
				ofn.lpstrFile = szDirect;
				*(ofn.lpstrFile) = 0;
				ofn.nMaxFile = sizeof(szDirect);
				ofn.lpstrFilter = NULL;
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = szFileName;
				*(ofn.lpstrFileTitle) = 0;
				ofn.nMaxFileTitle = sizeof(szFileName);
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_EXPLORER;
				GetOpenFileName(&ofn);
				break;
			case MENU_FILE_SAVELOCATION:
				//

				break;
			case MENU_FILE_SAVEASLOCATION:
				// Диалог сохранения

				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = NULL;
				ofn.lpstrFile = szDirect;
				*(ofn.lpstrFile) = 0;
				ofn.nMaxFile = sizeof(szDirect);
				ofn.lpstrFilter = NULL;
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = szFileName;
				*(ofn.lpstrFileTitle) = 0;
				ofn.nMaxFileTitle = sizeof(szFileName);
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_EXPLORER;
				GetSaveFileName(&ofn);
				break;
			case MENU_FILE_QUIT:
				// Выход
				PostQuitMessage(0);
				break;

			case MENU_PROJECT_IMPORT_MODEL:
				//
				break;
			case MENU_PROJECT_IMPORT_TEXTURE:
				//
				break;
			}

			if (lParam == (LPARAM)hBtn_Shader)
			{
				
			}
			else if (lParam == (LPARAM)hBtn_OpenModel)
			{
				
			}
			else if (lParam == (LPARAM)hBtn_ShowModel)
			{
				
			}
			else if (lParam == (LPARAM)hBtn_CloseModel)
			{

			}

			break;

		case WM_SIZE:
			if (wParam == SIZE_MAXIMIZED)
			{
				WinApi::isFullscreen = true;
				break;
			}
			else if (wParam == SIZE_RESTORED)
			{
				WinApi::isFullscreen = false;
			}

			windowWidth = LOWORD(lParam);
			windowHeight = HIWORD(lParam);
			break;
		
		case WM_MOVE:
			windowPositionX = LOWORD(lParam);
			windowPositionY = HIWORD(lParam);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT WndRenderProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (!Engine::isLoaded)
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		BindKey(hWnd, message, wParam, lParam);

		switch (message)
		{
		case WM_SIZE:

			windowRenderWidth = LOWORD(lParam);
			windowRenderHeight = HIWORD(lParam);

			glViewport(0, 0, windowRenderWidth, windowRenderHeight);
			break;

		case WM_MOVE:
			windowRenderPositionX = LOWORD(lParam);
			windowRenderPositionY = HIWORD(lParam);
			break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	LRESULT WndLocationProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (!Engine::isLoaded)
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		BindKey(hWnd, message, wParam, lParam);

		switch (message)
		{
		case WM_SIZE:
			windowLocationWidth = LOWORD(lParam);
			windowLocationHeight = HIWORD(lParam);

			MoveWindow(hWndListViewLocation, 0, 0, windowLocationWidth, windowLocationHeight, true);
			break;

		case WM_MOVE:
			windowLocationPositionX = LOWORD(lParam);
			windowLocationPositionY = HIWORD(lParam);

			MoveWindow(hWndListViewLocation, 0, 0, windowLocationWidth, windowLocationHeight, true);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default: // Все необработанные сообщения обработает сама Windows
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}
	LRESULT WndProjectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (!Engine::isLoaded)
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		BindKey(hWnd, message, wParam, lParam);

		switch (message)
		{
		case WM_SIZE:
			windowProjectWidth = LOWORD(lParam);
			windowProjectHeight = HIWORD(lParam);

			if (windowLocationHeight < 128)
			{
				windowLocationHeight = 128;
			}

			MoveWindow(hWndListViewProject, 0, 0, windowProjectWidth, windowProjectHeight, true);
			break;

		case WM_MOVE:
			windowProjectPositionX = LOWORD(lParam);
			windowProjectPositionY = HIWORD(lParam);

			MoveWindow(hWndListViewProject, 0, 0, windowProjectWidth, windowProjectHeight, true);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default: // Все необработанные сообщения обработает сама Windows
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	// Метод с циклом программы
	void Loop(GameObject *models, const uint16_t countModels)
	{
		MSG message{ 0 }; 	// Структура сообщения к окну
		int8_t iResult;		// Код состояния
		
		// Пока есть сообщения
		while ((iResult = GetMessage(&message, NULL, 0, 0)))
		{
			// Если система вернула отрицательный код (ошибка), то выходим из цикла обработки
			if (iResult < 0)
			{
				break;
			}

			// Обрабатываем сообщения в WndProc
			TranslateMessage(&message);
			DispatchMessage(&message);

			// Очистка
			glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (uint16_t i = 0; i < countModels; ++i)
			{
				models[i].Update();
				models[i].Render(windowRenderWidth, windowRenderHeight);
			}

			// Смена буфера
			SwapBuffers(hDC);
		}
	}
};
