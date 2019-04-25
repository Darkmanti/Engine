#include "WinApi.h"

// Первый define относится к загрузке шрифтов, второй к картинкам
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include <STB/stb_image.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

#include "Camera.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

#include "GameObject.h"
#include "FontObject.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

	HANDLE				debugConsole;

	OPENFILENAME		OFN{ 0 };
	LPSTR				filePath;

	char				szDirect[MAX_PATH],
						szFileName[MAX_PATH];

	bool				isLoaded;

	int64_t				mouseOffsetX,
						mouseOffsetY,
						lastMousePosX,
						lastMousePosY;

	POINT				mousePos{};

	RECT				windowRenderRect{},
						windowEngineRect{};

	bool				isCameraAction;

	HINSTANCE			hInstance;

	extern const uint16_t NumberOfKeys = 256;

	bool previousKeyboardState[NumberOfKeys];

	std::string				lastProjectFileName = "MyProject",
							lastLocationFileName = "MyLocation",
							projectPath;

	// Временная функции ====================================================
	GLfloat deltaTime = 0.016f;
	Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));
	void loadImage(GLuint &texture, char const* fileName, int Par);
	// Временная функции ====================================================

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
		pWndEngineClassEx.hInstance = hInstance;									// Дескриптор приложения
		pWndEngineClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);					// Подгружам курсор
		pWndEngineClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);				// Указатель на кисть с цветом фона (Типо кисть - рисование)
		pWndEngineClassEx.lpszClassName = "WndEngineClass";							// Наименование класса
		pWndEngineClassEx.hIcon = LoadIcon(hInstance, "IDI_ENGINEICON");			// Иконка

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
		pWndRenderClassEx.cbSize = sizeof(WNDCLASSEX);											// Размер в байтах структуры класса
		pWndRenderClassEx.style = CS_VREDRAW | CS_HREDRAW | CS_BYTEALIGNCLIENT;					// Стиль окна
		pWndRenderClassEx.lpfnWndProc = WndRenderProc;											// Указатель на оконную процедуру
		pWndRenderClassEx.hInstance = hInstance;												// Дескриптор приложения
		pWndRenderClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);								// Подгружам курсор
		pWndRenderClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);							// Указатель на кисть с цветом фона (Типо кисть - рисование)
		pWndRenderClassEx.lpszClassName = "WndRenderClass";										// Наименование класса

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
			0, 0,																		// Позиция
			1366, 768,																	// Размер
			0,																			// Родительское окно
			0,																			// Меню
			hInstance,																	// Десприптор приложения
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
			256, 256,										// Позиция
			800, 600,										// Размер
			hWndEngine,										// Родительское окно
			0,												// Меню
			hInstance,										// Десприптор приложения
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

		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuFile, "&Сервис");
		AppendMenu(hPopMenuFile, MF_SEPARATOR, MENU_SERVICE_QUIT + 1, "");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_SERVICE_QUIT, "Выход\tAlt+F4");

		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuScene, "&Локация");
		AppendMenu(hPopMenuScene, MF_STRING, MENU_LOCATION_NEW, "Создать");
		AppendMenu(hPopMenuScene, MF_STRING, MENU_LOCATION_OPEN, "Открыть");
		AppendMenu(hPopMenuScene, MF_STRING, MENU_LOCATION_SAVE, "Сохранить");
		AppendMenu(hPopMenuScene, MF_STRING, MENU_LOCATION_SAVE, "Добавть объект");
		AppendMenu(hPopMenuScene, MF_STRING, MENU_LOCATION_SAVE, "Удалить объект");

		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuProject, "&Проект");
		AppendMenu(hPopMenuProject, MF_STRING, MENU_PROJECT_NEW, "Создать");
		AppendMenu(hPopMenuProject, MF_STRING, MENU_PROJECT_OPEN, "Открыть");
		AppendMenu(hPopMenuProject, MF_STRING, MENU_PROJECT_SAVE, "Сохранить");
		AppendMenu(hPopMenuProject, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuProjectImport, "&Импорт");
		AppendMenu(hPopMenuProjectImport, MF_STRING, MENU_PROJECT_IMPORT_MODEL, "Модель");
		AppendMenu(hPopMenuProjectImport, MF_STRING, MENU_PROJECT_IMPORT_TEXTURE, "Текстуру");
		AppendMenu(hPopMenuProject, MF_STRING, MENU_PROJECT_BUILD, "Собрать");
		AppendMenu(hPopMenuProject, MF_STRING, MENU_PROJECT_CLEAR, "Очистить");

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
		if (ShowWindow(hWndEngine, SW_MAXIMIZE)) return 2;
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
			case MENU_SERVICE_QUIT:
				PostQuitMessage(0);
				break;
			case MENU_PROJECT_NEW:
				Project::New(OFN);
				break;
			case MENU_PROJECT_SAVE:
				Project::Save(OFN);
				break;
			case MENU_PROJECT_OPEN:
				Project::Load(OFN);
				break;
			case MENU_LOCATION_NEW:
				Location::New(OFN);
				break;
			case MENU_LOCATION_SAVE:
				Location::Save(OFN);
				break;
			case MENU_LOCATION_OPEN:
				Location::Load();
				break;
			}

			break;
		case WM_MOVE:
		case WM_SIZE:
			GetWindowRect(hWndEngine, &windowEngineRect);
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

		if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
			return true;

		switch (message)
		{
		case WM_SIZE:
			glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));

			GetWindowRect(hWndRender, &windowRenderRect);

			/*Debug("x = "); Debug(std::to_string(windowRenderRect.left).c_str()); Debug("\n");
			Debug("y = "); Debug(std::to_string(windowRenderRect.top).c_str()); Debug("\n");

			Debug("width = "); Debug(std::to_string(windowRenderRect.right).c_str()); Debug("\n");
			Debug("height = "); Debug(std::to_string(windowRenderRect.bottom).c_str()); Debug("\n");*/

			break;
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

	// Метод с циклом программы
	void Loop()
	{
		// Инициализация параметров интерфейса =============================================================================
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(hWndRender);
		ImGui_ImplOpenGL3_Init("#version 330");

		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		// Инициализация параметров интерфейса =============================================================================

		MSG message{ 0 }; 	// Структура сообщения к окну

		// Временно здесь будет инициализация тестовой сцены ===========================================
		glewExperimental = GL_TRUE;
		glewInit();

		glViewport(0, 0, 800, 600);

		// Тест трафарета и глубины
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		GLfloat* vertices = new GLfloat[288]{ 
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,	 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  1.0f,  0.0f };

		Shader ourShader("Shader//shader.vs", "Shader//shader.fs");
		Shader fontShader("Shader//FontShader.vs", "Shader//FontShader.fs");
		Shader selectShader("Shader//SelectShader.vs", "Shader//SelectShader.fs");

		GLuint texture1, texture2, texture3, texture4, texture5, denisjpg;
		loadImage(texture1, "Resource/container.jpg", GL_RGB);
		loadImage(texture2, "Resource/container2.png", GL_RGB);
		loadImage(texture3, "Resource/Wood/wood.jpg", GL_RGB);
		loadImage(texture4, "Resource/Iron/iron.jpg", GL_RGB);
		loadImage(texture5, "Resource/Bereza/Bereza.png", GL_RGBA);
		loadImage(denisjpg, "Resource/denis/denis.jpg", GL_RGB);

		 //Инициализация текста
		FontObject font1(&fontShader, 32, 256, "Resource/OpenSans-Regular.ttf", 32, 512, 512);

		// Инициализация объектов
		GameObject object1(vertices, 288, &ourShader, texture1);
		GameObject object2(vertices, 288, &ourShader, texture2);
		GameObject object3(&ourShader, &selectShader, "Resource/Wood/Wood.obj", texture3);
		GameObject object4(&ourShader, &selectShader, "Resource/Iron/iron.obj", texture4);
		GameObject object5(&ourShader, &selectShader, "Resource/Bereza/Bereza.obj", texture5);
		GameObject denis(&ourShader, "D:/Engine/Engine/Engine/Resource/denis");
		denis.setModel(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(-20.0f, -20.0f, 20.0f), 9.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		// Типо выбрали бревно - пока не работает как надо и это доработается когда будут меши
		//object3.isSelect = true;

		// Матрицы
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(camera.Zoom, (GLfloat)800 / (GLfloat)600, 0.1f, 1000.0f);
		glm::mat4 ortho(1.0f);
		ortho = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, 0.0f, 100.0f);

		unsigned int gameobject_count(0);
		GameObject* object_list = (GameObject*)malloc(0);

		// Временно здесь будет инициализация тестовой сцены ===========================================

		WinApi::Debug("Initialisation succes\n");

		// Пока есть сообщения
		// Если система вернула отрицательный код (ошибка), то выходим из цикла обработки
		while (true)
		{
			if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				if (message.message == WM_QUIT)
				{
					break;
				}

				// Обрабатываем сообщения в WndProc
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			CameraControllAction();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			if (isKeyDown(VK_G))
			{
				object_list = (GameObject*)realloc(object_list, sizeof(GameObject) * (gameobject_count + 1));
				GameObject temp(&ourShader, "D:/Engine/Engine/Engine/Resource/barrels");
				temp.setModel(glm::vec3(30.0f, 30.0f, 30.0f), glm::vec3(-20.0f, -20.0f, 20.0f), 9.0f, glm::vec3(0.0f, 1.0f, 0.0f));
				object_list[gameobject_count] = temp;
				++gameobject_count;
			}

			// Временный прорисовка =================================================================
			/*GLfloat currentFrame = GetProcessTimes(); НУЖНО ВЗЯТЬ ВРЕМЯ РАБОТЫ!!!
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;*/

			glm::mat4 view = glm::mat4(1.0f);
			view = camera.GetViewMatrix();

			for (int i = 0; i < gameobject_count; i++)
			{
				object_list[i].Draw(projection, view, camera.Position);
			}
			
			object1.DrawArray(projection, view, camera.Position);
			object2.setModel(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(5.0f, 1.0f, 1.0f), 9.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			object2.DrawArray(projection, view, camera.Position);

			object3.setModel(glm::vec3(30.0f, 30.0f, 30.0f), glm::vec3(5.0f, 1.0f, 1.0f), 9.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			object3.DrawArray_temp(projection, view, camera.Position);

			object4.setModel(glm::vec3(30.0f, 30.0f, 30.0f), glm::vec3(-50.0f, 1.0f, 1.0f), 9.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			object4.DrawArray_temp(projection, view, camera.Position);

			object5.setModel(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(-25.0f, 0.0f, 20.0f), 9.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			object5.DrawArray_temp(projection, view, camera.Position);

			glBindTexture(GL_TEXTURE_2D, denisjpg);
			denis.Draw(projection, view, camera.Position);
			glBindTexture(GL_TEXTURE_2D, 0);

			font1.Print(100, 500, "PARAWOZIK", glm::vec3(0.0f, 1.0f, 0.0f), ortho);
			// Временный прорисовка =================================================================


			// Интерфейс отрисовка =========================================================================================
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			// 3. Show another simple window.
			if (show_another_window)
			{
				ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
				ImGui::Text("Hello from another window!");
				if (ImGui::Button("Close Me"))
					show_another_window = false;
				ImGui::End();
			}

			// Вызов рендера интерфейса
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			// Интерфейс отрисовка =========================================================================================

			SwapBuffers(hDC);
		}
	}

	void InitInput()
	{
		for (uint16_t keyNum = 0; keyNum < NumberOfKeys; ++keyNum)
		{
			previousKeyboardState[keyNum] = isKeyDown(keyNum);
		}
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

	void CameraControllAction()
	{
		GetCursorPos(&mousePos);

		if (mousePos.x >= windowRenderRect.left && mousePos.y >= windowRenderRect.top && mousePos.x <= windowRenderRect.right && mousePos.y <= windowRenderRect.bottom)
		{
			// Если нажатие клавиш управления происходит впервые
			if (isKeyFirstPressed(VK_RBUTTON) || isKeyFirstPressed(VK_LBUTTON) && isKeyDown(VK_LMENU))
			{
				isCameraAction = true;

				lastMousePosX = mousePos.x;
				lastMousePosY = mousePos.y;
			}
		}

		if ((isKeyDown(VK_RBUTTON) || isKeyDown(VK_LBUTTON) && isKeyDown(VK_LMENU)) && isCameraAction)
		{
			if (isKeyDown(VK_W))
			{
				camera.ProcessKeyboard(FORWARD, deltaTime);
			}

			if (isKeyDown(VK_S))
			{
				camera.ProcessKeyboard(BACKWARD, deltaTime);
			}

			if (isKeyDown(VK_A))
			{
				camera.ProcessKeyboard(LEFT, deltaTime);
			}

			if (isKeyDown(VK_D))
			{
				camera.ProcessKeyboard(RIGHT, deltaTime);
			}

			mouseOffsetX = mousePos.x - lastMousePosX;
			mouseOffsetY = lastMousePosY - mousePos.y;

			//Debug("mousepos.x = "); Debug(std::to_string(mousePos.x).c_str()); Debug("\t");
			//Debug("last.x = "); Debug(std::to_string(lastMousePosX).c_str()); Debug("\t");
			//Debug("offset.x = "); Debug(std::to_string(mouseOffsetX).c_str()); Debug("\t");

			//Debug("\n");

			//Debug("mousepos.y = "); Debug(std::to_string(mousePos.y).c_str()); Debug("\t");
			//Debug("last.y = "); Debug(std::to_string(lastMousePosY).c_str()); Debug("\t");
			//Debug("offset.y = "); Debug(std::to_string(mouseOffsetY).c_str()); Debug("\t");

			//Debug("\n");

			lastMousePosX = mousePos.x;
			lastMousePosY = mousePos.y;

			camera.ProcessMouseMovement(mouseOffsetX, mouseOffsetY);
		}
		else
		{
			isCameraAction = false;
		}
	}

	void Debug(const char *sms)
	{
		WriteConsole(debugConsole, sms, strlen(sms), nullptr, NULL);
	}
	void Debug(int str)
	{
		char sms[256];
		_itoa(str, sms, 10);
		WriteConsole(debugConsole, sms, strlen(sms), nullptr, NULL);
	}
	void Debug(unsigned int str)
	{
		char sms[256];
		_itoa(str, sms, 10);
		WriteConsole(debugConsole, sms, strlen(sms), nullptr, NULL);
	}
	void Debug(float str)
	{
		char sms[256];
		std::stringstream iostr;
		iostr << str;
		iostr >> sms;
		WriteConsole(debugConsole, sms, strlen(sms), nullptr, NULL);
	}

	void Clear()
	{
		system("cls");
	}

	void loadImage(GLuint &texture, char const* fileName, int Par)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		stbi_set_flip_vertically_on_load(true);
		GLint w, h, comp;
		unsigned char* image = stbi_load(fileName, &w, &h, &comp, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, Par, w, h, 0, Par, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(image);
	}
};
