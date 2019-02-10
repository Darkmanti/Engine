#include "WinApi.h"

#include <CommCtrl.h>

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


	GLuint				vArrayObject,					// VAO
						vBufferObject;					// VBO


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

	Shader *shader;

	bool isEnabled;

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
		pWndRenderClassEx.style = CS_VREDRAW | CS_HREDRAW;					// Стиль окна
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
		hWndEngine = CreateWindowEx(WS_EX_ACCEPTFILES | WS_EX_WINDOWEDGE,				// Extended style
			pWndEngineClassEx.lpszClassName,											// Название класса
			"Engine",																	// Название окна
			WS_SYSMENU | WS_CAPTION | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,		// Стиль окна
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
		hWndRender = CreateWindowEx(NULL,					// Extended style
			pWndRenderClassEx.lpszClassName,				// Название класса
			"Рендер",										// Название окна
			WS_SIZEBOX | WS_CHILD,							// Стиль окна
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

		return 0;
	}
	uint16_t CreateWindowLocation()
	{
		hWndLocation = CreateWindowEx(NULL,
			pWndLocationClassEx.lpszClassName,
			"Локация",
			WS_CHILD | WS_SIZEBOX | WS_CAPTION | WS_ACTIVECAPTION,
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
			WS_CHILD | WS_SIZEBOX | WS_CAPTION | WS_ACTIVECAPTION,
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
		hWndListViewLocation = CreateWindowEx(NULL,
			WC_LISTVIEW,
			"",
			WS_VISIBLE | WS_CHILD | LVS_EDITLABELS,
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

		return 0;
	}
	uint16_t CreateListViewProject()
	{
		hWndListViewProject = CreateWindowEx(NULL,
			WC_LISTVIEW,
			"",
			WS_VISIBLE | WS_CHILD | LVS_EDITLABELS,
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
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_SAVELOCATION, "Сохранить\tCtrl+S");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_SAVEASLOCATION, "Сохранить как\tCtrl+Shift+S");
		AppendMenu(hPopMenuFile, MF_SEPARATOR, MENU_FILE_SAVEASLOCATION + 1, "");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_SAVEASLOCATION, "Выход\tAlt+F4");

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
			if (ShowWindow(hWndEngine, nCmdShow | SW_MAXIMIZE)) return 1;
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


		if (!ShowWindow(hWndListViewLocation, SW_SHOWNORMAL)) return 10;
		if (!ShowWindow(hWndListViewProject, SW_SHOWNORMAL)) return 11;

		// Показываем кнопки
		if (!ShowWindow(hBtn_OpenModel, SW_SHOWNORMAL)) return 12;
		if (!ShowWindow(hBtn_ShowModel, SW_SHOWNORMAL)) return 13;
		if (!ShowWindow(hBtn_Shader, SW_SHOWNORMAL)) return 14;
		if (!ShowWindow(hBtn_CloseModel, SW_SHOWNORMAL)) return 15;

		return 0;
	}

	// Оконные процедуры
	LRESULT WndEngineProc(HWND hWndEngine, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case MENU_FILE_NEWLOCATION:
				//
				break;
			case MENU_FILE_SAVELOCATION:
				//
				break;
			case MENU_FILE_SAVEASLOCATION:
				//
				break;
			case MENU_FILE_QUIT:
				//
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
				// Инициализация шейдера
				shader = new Shader("shader.vs", "shader.fs");	// Шейдер
			}
			else if (lParam == (LPARAM)hBtn_OpenModel)
			{
				// Обработка нажатия кнопки
				//GLdouble *vertices;

				//std::ifstream file;

				//file.open(Engine::dirAppData + std::string("\\models\\cube.obj"));

				//if (!file.is_open())
				//{
				//	break;
				//}

				//std::string str;

				//std::string::size_type size;

				//file >> str;

				//uint64_t countVertices(0); //Количество вертексов для 
				//countVertices = std::stoul(str, &size) * 3;

				//vertices = new double[countVertices];

				//uint64_t	countVertex(0);		// Итератор количества вертексов
				//uint8_t		countEmptyLines(0); // Количество пропущенных строк

				//while (!file.eof())
				//{
				//	file >> str;

				//	if (str[0] == '#')
				//	{
				//		getline(file, str);
				//		countEmptyLines = 0;
				//		continue;
				//	}

				//	if (str[0] == 'v' && str[1] != 'n')
				//	{
				//		double x, y, z;

				//		file >> str;
				//		x = std::stod(str, &size);

				//		file >> str;
				//		y = std::stod(str, &size);

				//		file >> str;
				//		z = std::stod(str, &size);

				//		vertices[countVertex] = x; ++countVertex;
				//		vertices[countVertex] = y; ++countVertex;
				//		vertices[countVertex] = z; ++countVertex;

				//		countEmptyLines = 0;
				//		continue;
				//	}

				//	if (countEmptyLines >= 8)
				//	{
				//		break;
				//	}

				//	getline(file, str);

				//	++countEmptyLines;
				//}

				//file.close();


				//Вертексы
				GLfloat *vertices = new GLfloat[9]{
					-1.f, -1.f, 0.f,
					1.f, -1.f, 0.f,
					0.f,  1.f, 0.f
				};

				Graphics::CreateObject(vertices, vArrayObject, vBufferObject);

				delete[]vertices;
			}
			else if (lParam == (LPARAM)hBtn_ShowModel)
			{
				isEnabled = true;
			}
			else if (lParam == (LPARAM)hBtn_CloseModel)
			{
				glDeleteBuffers(1, &vBufferObject);
				isEnabled = false;
			}

			break;

		case WM_LBUTTONUP:
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

		default: // Все необработанные сообщения обработает сама Windows
			return DefWindowProc(hWndEngine, message, wParam, lParam);
		}

		return 0;
	}
	LRESULT WndRenderProc(HWND hWndRender, UINT message, WPARAM wParam, LPARAM lParam)
	{
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

		default: // Все необработанные сообщения обработает сама Windows
			return DefWindowProc(hWndRender, message, wParam, lParam);
		}

		return 0;
	}
	LRESULT WndLocationProc(HWND hWndEngine, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND:
			break;

		case WM_LBUTTONUP:
			break;

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
			return DefWindowProc(hWndEngine, message, wParam, lParam);
		}

		return 0;
	}
	LRESULT WndProjectProc(HWND hWndEngine, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND:
			break;

		case WM_LBUTTONUP:
			break;

		case WM_SIZE:
			windowProjectWidth = LOWORD(lParam);
			windowProjectHeight = HIWORD(lParam);

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
			return DefWindowProc(hWndEngine, message, wParam, lParam);
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

			if (isEnabled)
			{
				// Использование шейдера
				shader->use();

				// Вывод графики
				Graphics::DrawObject(vArrayObject);
			}

			// Смена буфера
			SwapBuffers(hDC);
		}
	}
};
