#include "Output.h"

#include "GLEW/glew.h"

#include <windows.h>
#include <stdint.h>

#include "Engine.h"
#include "Input.h"

namespace Output
{
	 GLuint			windowWidth;			// Ширина окна редактора
	 GLuint			windowHeight;			// Высота окна редактора

	 GLuint			windowRenderWidth;		// Ширина окна рендера. Изменяется относительно размера
	 GLuint			windowRenderHeight;		// Высота окна рендера. Изменяется относительно размера


	 WNDCLASSEX		pWndClassEx;			// Структура класса окна


	 GLuint			vArrayObject;			// VAO
	 GLuint			vBufferObject;			// VBO
	 GLuint			shaderProgram;			// Шейдерная программа


	// Различные дескрипторы
	 HDC			hDC;						// Дескриптор устройства
	 HGLRC			hRC;						// Дескпритор ...

	 HWND hWnd;									// Главное окно редактора
	 HWND hWindowRender;						// Окно рендера внутри редактора

	// Регистрация класса
	ATOM RegisterEngineWindow(const HINSTANCE hInstance)
	{
		// Описываем поля структур
		pWndClassEx.cbSize = sizeof(WNDCLASSEX),						// Размер в байтах структуры класса
		pWndClassEx.style = CS_VREDRAW | CS_HREDRAW;					// Стиль окна
		pWndClassEx.lpfnWndProc = WndProc;								// Указатель на оконную процедуру
		pWndClassEx.hInstance = hInstance;								// Дескриптор приложения
		pWndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);				// Подгружам курсор
		pWndClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);		// Указатель на кисть с цветом фона (Типо кисть - рисование)
		pWndClassEx.lpszClassName = "EngineWndClass\0";					// Наименование класса

		int16_t iError(RegisterClassEx(&pWndClassEx));

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
		HWND &hWnd(Output::hWnd);
		hWnd = CreateWindowEx(
			WS_EX_ACCEPTFILES | WS_EX_WINDOWEDGE,	// Extended style
			Output::pWndClassEx.lpszClassName,				// Название класса
			static_cast<LPCTSTR>("Engine\0"),		// Название окна
			WS_SYSMENU | WS_CAPTION | WS_SIZEBOX |
			WS_MINIMIZEBOX | WS_MAXIMIZEBOX,		// Стиль окна
			0, 0,									// Позиция
			windowWidth, windowHeight,				// Размер
			0,										// Еще какая-то залупа
			0,										// И еще что-то там
			Engine::hInstance,						// Десприптор приложения
			0										// Все это говно доступно на msdn
		);

		// Проверка на создание окна
		if (!hWnd)
		{
			MessageBox(hWnd, "Ошибка", "Окно не создано", MB_OK);
			return 0;
		}

		// Подгружаем остальной интерфейс (кнопки, хуепки, inputText и пр.)
		if (InitInterface())
		{
			MessageBox(hWnd, "Ошибка", "Интерфейс не создан", MB_OK);

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
		case WM_COMMAND:
			if (lParam == (LPARAM)Input::hBtn_OpenModel)
			{
				//// Обработка нажатия кнопки
				//GLdouble *vertices;

				//std::ifstream file;

				//file.open(getDirAppData() + static_cast<std::string>("\\models\\cube.obj"));

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
				GLfloat vertices[]{
					-1.f, -1.f, 0.f,
					1.f, -1.f, 0.f,
					0.f,  1.f, 0.f
				};

				glGenVertexArrays(1, &vArrayObject);
				glBindVertexArray(vArrayObject);

				glGenBuffers(1, &vBufferObject); // Сгенерировали ID объекта буфера
				glBindBuffer(GL_ARRAY_BUFFER, vBufferObject); // Привязка буфера
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // GL_ARRAY_BUFFER, 

				// Обычно должно стоять сразу после Шейдерной программы
				glEnableVertexAttribArray(0); // Указываем, что первым буфером атрибутов будут вершины
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
				//glBindVertexArray(0);

				//delete	[]vertices;
			}
			break;

		case WM_LBUTTONUP: // Реакция на сообщение
			MessageBox(hWnd, "Вы кликнули!", "Событие", MB_OK);
			break;

		case WM_SIZE: // Реакция на смену размера окна
			MoveWindow(hWindowRender, 0, Input::sizeButton, LOWORD(lParam), HIWORD(lParam), true);
			Output::windowWidth = LOWORD(lParam);
			Output::windowHeight = HIWORD(lParam);
			Output::windowRenderWidth = LOWORD(lParam);
			Output::windowRenderHeight = HIWORD(lParam) - Input::sizeButton;
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

			glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			glUseProgram(shaderProgram);
			glBindBuffer(GL_ARRAY_BUFFER, vBufferObject);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLES, 0, 3);
			SwapBuffers(Output::hDC);
		}
	}

	// Инициализация интерфейса
	int16_t InitInterface()
	{
		// ViewportRender
		{
			HWND &hBtn(hWindowRender);
			hBtn = CreateWindow("BUTTON", "",
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				0, Input::sizeButton, windowRenderWidth, windowRenderHeight, hWnd, 0, Engine::hInstance, NULL);

			// Возврат номера интерфейса который не создан
			if (!hBtn)
			{
				return 1;
			}

			ShowWindow(hBtn, SW_NORMAL);
		}

		// Кнопка загрузки модели
		{
			HWND &hBtn(Input::hBtn_OpenModel);
			hBtn = CreateWindow("BUTTON", "Открыть модель",
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				0, 0, Input::sizeButton, Input::sizeButton, hWnd, 0, Engine::hInstance, NULL);

			if (!hBtn)
			{
				return 2;
			}

			ShowWindow(hBtn, SW_NORMAL);
		}

		{

		}

		return 0;
	}

	void EnableOpenGL()
	{
		PIXELFORMATDESCRIPTOR pfd;

		int iFormat;

		hDC = GetDC(hWindowRender);

		ZeroMemory(&pfd, sizeof(pfd));

		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 16;
		pfd.iLayerType = PFD_MAIN_PLANE;
		iFormat = ChoosePixelFormat(hDC, &pfd);
		SetPixelFormat(hDC, iFormat, &pfd);

		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
	}

	void DisableOpenGL()
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRC);
		ReleaseDC(hWnd, hDC);
	}
};
