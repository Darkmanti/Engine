// Файл содержит в себе члены блока инициализации главного окна
#include "GLEW/glew.h"

#include "globals.h"
#include "interface.h"
#include "buttons.h"

#include <windows.h>
#include <stdint.h>
#include <fstream>
#include <string>

// Объявили будущий метод для последующей передачи его указателя в структуру WNDCLASSA
LRESULT WndProc(HWND, uint32_t, WPARAM, LPARAM);

// Регистрация окна для семантической группировки стейтментов
ATOM RegisterEngineWindow(const HINSTANCE hInstance)
{
	// Описываем поля структур
	getWndClassA().style = CS_VREDRAW | CS_HREDRAW;				// Стиль окна
	getWndClassA().lpfnWndProc = WndProc;						// Указатель на оконную процедуру
	getWndClassA().hInstance = hInstance;						// Дескриптор приложения
	getWndClassA().hCursor = LoadCursor(NULL, IDC_ARROW);		// Подгружам курсор
	getWndClassA().hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);	// Указатель на кисть с цветом фона (Типо кисть - рисование)
	getWndClassA().lpszClassName = "EngineWndClass\0";			// Наименование класса

	int16_t iError(RegisterClass(&getWndClassA()));

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
	HWND &hWnd(getHWnd());
	hWnd = CreateWindow(
		getWndClassA().lpszClassName,				// Название класса
		static_cast<LPCTSTR>("Engine\0"),		// Название окна
		WS_SYSMENU | WS_CAPTION | WS_SIZEBOX |
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX,		// Стиль окна
		0, 0,									// Позиция
		getWindowWidth(), getWindowHeight(),	// Размер
		0,										// Еще какая-то залупа
		0,										// И еще что-то там
		getHInstance(),							// Десприптор приложения
		0										// Все это говно доступно на msdn
	);

	if (!getHWnd())
	{
		MessageBox(getHWnd(), "Ошибка", "Окно не создано", MB_OK);
		return 0;
	}

	if (InitInterface())
	{
		MessageBox(getHWnd(), "Ошибка", "Интерфейс не создан", MB_OK);

		return 0;
	}

	// Показать окно
	ShowWindow(getHWnd(), nCmdShow);

	// Обновлять окно
	UpdateWindow(getHWnd());

	return 1;
}

// Функция обработчика сообщений (Процедура окна)
LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		if (lParam == (LPARAM)getHBtnOpenModel())
		{
			// Обработка нажатия кнопки
			GLfloat *vertices;

			std::ifstream file;

			file.open(getDirAppData() + static_cast<std::string>("cube.obj"));

			if (!file.is_open())
			{
				break;
			}

			std::string str;

			uint64_t	countVertex(0);		// Итератор количества вертексов
			uint8_t		countEmptyLines(0); // Количество пропущенных строк

			std::string::size_type size;

			file >> str;

			uint64_t countVertices(0); //Количество вертексов для 
			countVertices = std::stod(str, &size);

			vertices = new GLfloat[countVertices];

			while (!file.eof())
			{
				file >> str;

				if (str[0] == '#')
				{
					getline(file, str);
					countEmptyLines = 0;
					continue;
				}

				if (str[0] == 'v' && str[1] != 'n')
				{
					double vertex;

					file >> str;
					vertex = std::stod(str, &size);

					vertices[countVertex] = vertex;
					++countVertex;

					countEmptyLines = 0;
					continue;
				}

				if (countEmptyLines >= 8)
				{
					break;
				}

				getline(file, str);

				++countEmptyLines;
			}

			file.close();

			//Вертексы
			//GLfloat vertices[]{
			//	-1.f, -1.f, 0.0f,
			//	1.f, -1.f, 0.0f,
			//	0.0f,  1.f, 0.0f
			//};

			glGenVertexArrays(1, &getVArrayObject());
			glBindVertexArray(getVArrayObject());

			// VBO
			GLuint vBufferObject;

			glGenBuffers(1, &vBufferObject); // Сгенерировали ID объекта буфера
			glBindBuffer(GL_ARRAY_BUFFER, vBufferObject); // Привязка буфера
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // GL_ARRAY_BUFFER, 

			// Обычно должно стоять сразу после Шейдерной программы
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // 
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);
		}
		break;
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

		glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(getShaderProgram());
		glBindVertexArray(getVArrayObject());
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		SwapBuffers(getHDC());

		
	}
}
