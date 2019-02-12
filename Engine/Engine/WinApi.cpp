#include "WinApi.h"

#include <CommCtrl.h>
#include <algorithm>

namespace WinApi
{
	int32_t				windowPositionX,				// ������� ���� �� X
						windowPositionY,				// ������� ���� �� Y
						windowWidth,					// ������ ���� ���������
						windowHeight,					// ������ ���� ���������

						windowRenderPositionX,			// ������� ���� ������� �� X
						windowRenderPositionY,			// ������� ���� ������� �� Y
						windowRenderWidth,				// ������ ���� �������. ���������� ������������ �������
						windowRenderHeight,				// ������ ���� �������. ���������� ������������ �������

						windowLocationPositionX,		// ������� ���� location �� X
						windowLocationPositionY,		// ������� ���� location �� Y
						windowLocationWidth,			// ������ ���� location
						windowLocationHeight,			// ������ ���� location

						windowProjectPositionX,			// ������� ���� location �� X
						windowProjectPositionY,			// ������� ���� location �� Y
						windowProjectWidth,				// ������ ���� location
						windowProjectHeight,			// ������ ���� location

						isFullscreen,					// ����������� ���� ��� ���
						buttonsSize;					// ������ ������ �� ������ ������������


	WNDCLASSEX			pWndEngineClassEx,				// ��������� ������ ����
						pWndRenderClassEx,				// ��������� ������ ������ ����
						pWndLocationClassEx,			// ��������� ������ location
						pWndProjectClassEx;				// ��������� ������ location


	GLuint				vbo, vao, ebo;


	// ��������� �����������
	HDC					hDC;							// ���������� ����������
	HGLRC				hRC;							// ���������� ...

	HWND				hWndEngine,						// ������� ���� ���������
						hWndRender,						// ���� ������� ������ ���������
						hWndLocation,					// ���� �������
						hWndProject,					// ���� �������

						hWndListViewLocation,			// ListView �������
						hWndListViewProject,			// ListView �������

						hStatusWindow,

						// ������
						hBtn_Shader,
						hBtn_OpenModel,
						hBtn_ShowModel,
						hBtn_CloseModel;

						// ����
	HMENU				hMenu,
						hPopMenuFile,
						hPopMenuProject,
						hPopMenuProjectImport;

	// ����������� ������ ����
	ATOM RegisterWindowEngine()
	{
		// ��������� ���� ��������
		pWndEngineClassEx.cbSize = sizeof(WNDCLASSEX);								// ������ � ������ ��������� ������
		pWndEngineClassEx.style = CS_VREDRAW | CS_HREDRAW;							// ����� ����
		pWndEngineClassEx.lpfnWndProc = WndEngineProc;								// ��������� �� ������� ���������
		pWndEngineClassEx.hInstance = Engine::hInstance;							// ���������� ����������
		pWndEngineClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);					// ��������� ������
		pWndEngineClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);				// ��������� �� ����� � ������ ���� (���� ����� - ���������)
		pWndEngineClassEx.lpszClassName = "WndEngineClass";							// ������������ ������
		pWndEngineClassEx.hIcon = LoadIcon(Engine::hInstance, "IDI_ENGINEICON");	// ������

		if (int16_t iError = RegisterClassEx(&pWndEngineClassEx))
		{

		}
		else
		{
			MessageBox(NULL, "����� �� ���������������", "RegisterWindowEngine", MB_OK);
			return iError;
		}

		return 0;
	}
	ATOM RegisterWindowRender()
	{
		pWndRenderClassEx.cbSize = sizeof(WNDCLASSEX);						// ������ � ������ ��������� ������
		pWndRenderClassEx.style = CS_VREDRAW | CS_HREDRAW;					// ����� ����
		pWndRenderClassEx.lpfnWndProc = WndRenderProc;						// ��������� �� ������� ���������
		pWndRenderClassEx.hInstance = Engine::hInstance;					// ���������� ����������
		pWndRenderClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);			// ��������� ������
		pWndRenderClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);		// ��������� �� ����� � ������ ���� (���� ����� - ���������)
		pWndRenderClassEx.lpszClassName = "WndRenderClass";					// ������������ ������

		if (int16_t iError = RegisterClassEx(&pWndRenderClassEx))
		{

		}
		else
		{
			MessageBox(NULL, "����� �� ���������������", "RegisterWindowRender", MB_OK);
			return iError;
		}

		return 0;
	}
	ATOM RegisterWindowLocation()
	{
		pWndLocationClassEx.cbSize = sizeof(WNDCLASSEX);							// ������ � ������ ��������� ������
		pWndLocationClassEx.style = CS_VREDRAW | CS_HREDRAW;						// ����� ����
		pWndLocationClassEx.lpfnWndProc = WndLocationProc;							// ��������� �� ������� ���������
		pWndLocationClassEx.hInstance = Engine::hInstance;							// ���������� ����������
		pWndLocationClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);					// ��������� ������
		pWndLocationClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);			// ��������� �� ����� � ������ ���� (���� ����� - ���������)
		pWndLocationClassEx.lpszClassName = "WndLocationClass";						// ������������ ������

		if (int16_t iError = RegisterClassEx(&pWndLocationClassEx))
		{

		}
		else
		{
			MessageBox(NULL, "����� �� ���������������", "RegisterWindowLocation", MB_OK);
			return iError;
		}

		return 0;
	}
	ATOM RegisterWindowProject()
	{
		pWndProjectClassEx.cbSize = sizeof(WNDCLASSEX);								// ������ � ������ ��������� ������
		pWndProjectClassEx.style = CS_VREDRAW | CS_HREDRAW;							// ����� ����
		pWndProjectClassEx.lpfnWndProc = WndProjectProc;							// ��������� �� ������� ���������
		pWndProjectClassEx.hInstance = Engine::hInstance;							// ���������� ����������
		pWndProjectClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);					// ��������� ������
		pWndProjectClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);				// ��������� �� ����� � ������ ���� (���� ����� - ���������)
		pWndProjectClassEx.lpszClassName = "WndProjectClass";						// ������������ ������

		if (int16_t iError = RegisterClassEx(&pWndProjectClassEx))
		{

		}
		else
		{
			MessageBox(NULL, "����� �� ���������������", "RegisterWindowProject", MB_OK);
			return iError;
		}

		return 0;
	}

	// �������� ����
	uint16_t CreateWindowEngine()
	{
		// ���������� ����
		hWndEngine = CreateWindowEx(WS_EX_ACCEPTFILES | WS_EX_WINDOWEDGE,				// Extended style
			pWndEngineClassEx.lpszClassName,											// �������� ������
			"������",																	// �������� ����
			WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,		// ����� ����
			windowPositionX, windowPositionY,											// �������
			windowWidth, windowHeight,													// ������
			0,																			// ������������ ����
			0,																			// ����
			Engine::hInstance,															// ���������� ����������
			0																			// ��� ��� ����� �������� �� msdn
		);

		// �������� �� �������� ����
		if (!hWndEngine)
		{
			MessageBox(NULL, "���� �� �������", "������", MB_OK);
			return 1;
		}

		return 0;
	}
	uint16_t CreateWindowRender()
	{
		hWndRender = CreateWindowEx(NULL,					// Extended style
			pWndRenderClassEx.lpszClassName,				// �������� ������
			"������",										// �������� ����
			WS_SIZEBOX | WS_CHILD,							// ����� ����
			windowRenderPositionX, windowRenderPositionY,	// �������
			windowRenderWidth, windowRenderHeight,			// ������
			hWndEngine,										// ������������ ����
			0,												// ����
			Engine::hInstance,								// ���������� ����������
			0												// ��� ��� ����� �������� �� msdn
		);

		// �������� �� �������� ����
		if (!hWndRender)
		{
			MessageBox(NULL, "���� �� �������", "������", MB_OK);
			return 1;
		}

		Engine::camera = new Camera(glm::vec3(0, 4, 100));

		return 0;
	}
	uint16_t CreateWindowLocation()
	{
		hWndLocation = CreateWindowEx(NULL,
			pWndLocationClassEx.lpszClassName,
			"�������",
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
			"������",
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

		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuFile, "&����");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_NEWLOCATION, "����� �������\tCtrl+N");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_SAVELOCATION, "���������\tCtrl+S");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_SAVEASLOCATION, "��������� ���\tCtrl+Shift+S");
		AppendMenu(hPopMenuFile, MF_SEPARATOR, MENU_FILE_SAVEASLOCATION + 1, "");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_SAVEASLOCATION, "�����\tAlt+F4");

		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuProject, "&������");
		AppendMenu(hPopMenuProject, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuProjectImport, "&������");
		AppendMenu(hPopMenuProjectImport, MF_STRING, MENU_PROJECT_IMPORT_MODEL, "������");
		AppendMenu(hPopMenuProjectImport, MF_STRING, MENU_PROJECT_IMPORT_TEXTURE, "��������");

		SetMenu(hWndEngine, hMenu);

		return 0;
	}
	// �������� ������ ����������
	uint16_t CreateButtons()
	{
		// ������ ���������� ��������� ���������
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

		// ������ �������� ������
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

		// ������ ��������� ������
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

		// ������ �������� ������
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

	// ������������� ����������
	uint16_t InitInterface()
	{
		uint16_t iError;

		if (iError = RegisterWindowEngine()) return iError * 10 + 1;
		if (iError = RegisterWindowRender()) return iError * 10 + 2;
		if (iError = RegisterWindowLocation()) return iError * 10 + 3;
		if (iError = RegisterWindowProject()) return iError * 10 + 4;

		// �������� �� �������� ����
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


		if (!ShowWindow(hWndListViewLocation, SW_SHOWNORMAL)) return 10;
		if (!ShowWindow(hWndListViewProject, SW_SHOWNORMAL)) return 11;

		// ���������� ������
		if (!ShowWindow(hBtn_OpenModel, SW_SHOWNORMAL)) return 12;
		if (!ShowWindow(hBtn_ShowModel, SW_SHOWNORMAL)) return 13;
		if (!ShowWindow(hBtn_Shader, SW_SHOWNORMAL)) return 14;
		if (!ShowWindow(hBtn_CloseModel, SW_SHOWNORMAL)) return 15;

		return 0;
	}

	// ������� ���������
	LRESULT WndEngineProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (!Engine::isLoaded)
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

		default: // ��� �������������� ��������� ���������� ���� Windows
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}
	LRESULT WndRenderProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

		default: // ��� �������������� ��������� ���������� ���� Windows
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}
	LRESULT WndLocationProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

			SetWindowPos(hWndListViewLocation, HWND_TOPMOST, 0, 0, windowLocationWidth, windowLocationHeight, true);
			break;

		case WM_MOVE:
			windowLocationPositionX = LOWORD(lParam);
			windowLocationPositionY = HIWORD(lParam);

			SetWindowPos(hWndListViewLocation, HWND_TOPMOST, 0, 0, windowLocationWidth, windowLocationHeight, true);
			break;

		case WM_ACTIVATE:
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default: // ��� �������������� ��������� ���������� ���� Windows
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}
	LRESULT WndProjectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

			SetWindowPos(hWndListViewProject, HWND_TOPMOST, 0, 0, windowProjectWidth, windowProjectHeight, true);
			break;

		case WM_MOVE:
			windowProjectPositionX = LOWORD(lParam);
			windowProjectPositionY = HIWORD(lParam);

			SetWindowPos(hWndListViewProject, HWND_TOPMOST, 0, 0, windowProjectWidth, windowProjectHeight, true);
			break;

		case WM_ACTIVATE:
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default: // ��� �������������� ��������� ���������� ���� Windows
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	// ����� � ������ ���������
	void Loop(GameObject *models, const uint16_t countModels)
	{
		MSG message{ 0 }; 	// ��������� ��������� � ����
		int8_t iResult;		// ��� ���������
		
		// ���� ���� ���������
		while ((iResult = GetMessage(&message, NULL, 0, 0)))
		{
			// ���� ������� ������� ������������� ��� (������), �� ������� �� ����� ���������
			if (iResult < 0)
			{
				break;
			}

			// ������������ ��������� � WndProc
			TranslateMessage(&message);
			DispatchMessage(&message);

			// �������
			glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (uint16_t i = 0; i < countModels; ++i)
			{
				models[i].Update();
				models[i].Render(windowRenderWidth, windowRenderHeight);
			}

			// ����� ������
			SwapBuffers(hDC);
		}
	}
};
