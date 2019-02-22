#include "WinApi.h"

#include <CommCtrl.h>
#include <algorithm>
#include <thread>
#include <windowsx.h>

// ��������� ������� ===========================================
#include "Camera.h"
#include "Shader.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
// ��������� ������� ===========================================

#pragma comment(lib,"ComCtl32.Lib")

namespace WinApi
{
	WNDCLASSEX			pWndEngineClassEx,				// ��������� ������ ����
						pWndRenderClassEx;				// ��������� ������ ������ ����

	// ��������� �����������
	HDC					hDC;							// ���������� ����������
	HGLRC				hRC;							// ���������� ...

	HWND				hWndEngine,						// ������� ���� ���������
						hWndRender,						// ���� ������� ������ ���������

						hWndListViewLocation,			// ListView �������
						hWndListViewProject;			// ListView �������

	// ����
	HMENU				hMenu,
						hPopMenuFile,
						hPopMenuScene,
						hPopMenuProject,
						hPopMenuProjectImport;

	OPENFILENAME		OFN{ 0 };

	char				szDirect[MAX_PATH],
						szFileName[MAX_PATH];

	bool				isLoaded,
						isFirstMouse;

	int16_t				mousePositionX,
						mousePositionY,
						xpos,
						ypos,
						lastX,
						lastY;

	HINSTANCE			hInstance;

	extern const uint16_t NumberOfKeys = 256;

	bool previousKeyboardState[NumberOfKeys];

	// ��������� ������� ====================================================
	void loadImage(GLuint &texture, char const* fileName);
	void Do_Movement();
	GLfloat deltaTime = 0.016f;
	Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));
	// ��������� ������� ====================================================

	bool ListViewAddItem(const char* elementName, HWND hWndListView)
	{
		LVITEM lvi;
		lvi.mask = LVIF_TEXT;
		lvi.iSubItem = 0;
		lvi.iItem = 0x7FFFFFFF;
		lvi.pszText = (LPSTR)elementName;

		// �������� �������
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

	// ����������� ������ ����
	ATOM RegisterWindowEngine()
	{
		// ��������� ���� ��������
		pWndEngineClassEx.cbSize = sizeof(WNDCLASSEX);								// ������ � ������ ��������� ������
		pWndEngineClassEx.style = CS_VREDRAW | CS_HREDRAW;							// ����� ����
		pWndEngineClassEx.lpfnWndProc = WndEngineProc;								// ��������� �� ������� ���������
		pWndEngineClassEx.hInstance = hInstance;									// ���������� ����������
		pWndEngineClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);					// ��������� ������
		pWndEngineClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);				// ��������� �� ����� � ������ ���� (���� ����� - ���������)
		pWndEngineClassEx.lpszClassName = "WndEngineClass";							// ������������ ������
		pWndEngineClassEx.hIcon = LoadIcon(hInstance, "IDI_ENGINEICON");			// ������

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
		pWndRenderClassEx.cbSize = sizeof(WNDCLASSEX);											// ������ � ������ ��������� ������
		pWndRenderClassEx.style = CS_VREDRAW | CS_HREDRAW | CS_BYTEALIGNCLIENT;					// ����� ����
		pWndRenderClassEx.lpfnWndProc = WndRenderProc;											// ��������� �� ������� ���������
		pWndRenderClassEx.hInstance = hInstance;												// ���������� ����������
		pWndRenderClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);								// ��������� ������
		pWndRenderClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);							// ��������� �� ����� � ������ ���� (���� ����� - ���������)
		pWndRenderClassEx.lpszClassName = "WndRenderClass";										// ������������ ������

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

	// �������� ����
	uint16_t CreateWindowEngine()
	{
		// ���������� ����
		hWndEngine = CreateWindowEx(WS_EX_ACCEPTFILES,									// Extended style
			pWndEngineClassEx.lpszClassName,											// �������� ������
			"������",																	// �������� ����
			WS_OVERLAPPEDWINDOW,														// ����� ����
			0, 0,																		// �������
			1366, 768,																	// ������
			0,																			// ������������ ����
			0,																			// ����
			hInstance,																	// ���������� ����������
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
		hWndRender = CreateWindowEx(WS_EX_ACCEPTFILES,		// Extended style
			pWndRenderClassEx.lpszClassName,				// �������� ������
			"������",										// �������� ����
			WS_THICKFRAME | WS_CHILD,						// ����� ����
			256, 256,										// �������
			800, 600,										// ������
			hWndEngine,										// ������������ ����
			0,												// ����
			hInstance,										// ���������� ����������
			0												// ��� ��� ����� �������� �� msdn
		);

		// �������� �� �������� ����
		if (!hWndRender)
		{
			MessageBox(NULL, "���� �� �������", "������", MB_OK);
			return 1;
		}

		return 0;
	}
	uint16_t CreateListViewLocation()
	{
		hWndListViewLocation = CreateWindowEx(NULL,
			WC_LISTVIEW,
			"�����",
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

		// �������������� ��������
		SetWindowSubclass(hWndListViewLocation, SubClassLocationProc, 1, NULL);
		ListView_SetExtendedListViewStyle(hWndListViewLocation, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		// �������� �������
		LVCOLUMN lvc;
		lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.iSubItem = 0;
		lvc.pszText = (LPSTR)"���";
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
			"������",
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

		// �������� �������
		LVCOLUMN lvc;
		lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.iSubItem = 0;
		lvc.pszText = (LPSTR)"���";
		lvc.cx = 100;

		if (ListView_InsertColumn(hWndListViewProject, 0, &lvc) == -1)
		{
			return 1;
		}

		lvc.pszText = (LPSTR)"���";
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

		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuFile, "&����");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_NEWLOCATION, "����� �������\tCtrl+N");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_OPENLOCATION, "������� �������\tCtrl+O");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_SAVELOCATION, "���������\tCtrl+S");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_SAVEASLOCATION, "��������� ���\tCtrl+Shift+S");
		AppendMenu(hPopMenuFile, MF_SEPARATOR, MENU_FILE_SAVEASLOCATION + 1, "");
		AppendMenu(hPopMenuFile, MF_STRING, MENU_FILE_QUIT, "�����\tAlt+F4");

		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuScene, "&�����");
		AppendMenu(hPopMenuScene, MF_STRING, MENU_SCENE_CREATE, "�������");
		AppendMenu(hPopMenuScene, MF_STRING, MENU_SCENE_DELETE, "�������");

		AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuProject, "&������");
		AppendMenu(hPopMenuProject, MF_STRING | MF_POPUP, (UINT_PTR)hPopMenuProjectImport, "&������");
		AppendMenu(hPopMenuProjectImport, MF_STRING, MENU_PROJECT_IMPORT_MODEL, "������");
		AppendMenu(hPopMenuProjectImport, MF_STRING, MENU_PROJECT_IMPORT_TEXTURE, "��������");

		SetMenu(hWndEngine, hMenu);

		return 0;
	}

	// ������������� ����������
	uint16_t InitInterface()
	{
		uint16_t iError;

		if (iError = RegisterWindowEngine()) return iError * 10 + 1;
		if (iError = RegisterWindowRender()) return iError * 10 + 2;

		// �������� �� �������� ����
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

	// ������� ���������
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
				// ������ ����������
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
				// �����
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
		case WM_SIZE:
			glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_MOUSEMOVE:
			if (wParam == MK_RBUTTON)
			{
				mousePositionX = GET_X_LPARAM(wParam);
				mousePositionY = GET_Y_LPARAM(wParam);
			}
			else if (wParam == MK_LBUTTON && GetKeyState(VK_LMENU) < 0)
			{
				mousePositionX = GET_X_LPARAM(wParam);
				mousePositionY = GET_Y_LPARAM(wParam);
			}
			break;
		case WM_RBUTTONDOWN:
			SetCapture(hWndRender);
			break;
		case WM_RBUTTONUP:
			ReleaseCapture();
			break;
		case WM_LBUTTONDOWN:
			if (GetKeyState(VK_LMENU) < 0)
			{
				SetCapture(hWndRender);
			}
			break;
		case WM_LBUTTONUP:
			ReleaseCapture();
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

	// ����� � ������ ���������
	void Loop()
	{
		MSG message{ 0 }; 	// ��������� ��������� � ����

		// �������� ����� ����� ������������� �������� ����� ===========================================
		glewExperimental = GL_TRUE;
		glewInit();

		glViewport(0, 0, 800, 600);
		glEnable(GL_DEPTH_TEST);

		GLfloat vertices[] = {
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
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  1.0f,  0.0f
		};

		Shader ourShader("Shader//shader.vs", "Shader//shader.fs");

		GLuint VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

		GLuint texture1;
		loadImage(texture1, "Resource/container.jpg");
		// �������� ����� ����� ������������� �������� ����� ===========================================

		// ���� ���� ���������
		// ���� ������� ������� ������������� ��� (������), �� ������� �� ����� ���������
		while (true)
		{
			if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				if (message.message == WM_QUIT)
				{
					break;
				}

				// ������������ ��������� � WndProc
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			Do_Movement();
			mouseMove();

			// ��������� ���������� =================================================================
			/*GLfloat currentFrame = GetProcessTimes(); ����� ����� ����� ������!!!
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;*/

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 view = glm::mat4(1.0f);
			view = camera.GetViewMatrix();
			glm::mat4 projection = glm::mat4(1.0f);
			projection = glm::perspective(camera.Zoom, (GLfloat)800 / (GLfloat)600, 0.1f, 1000.0f);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f));

			ourShader.use();
			ourShader.setUniform("projection", projection);
			ourShader.setUniform("model", model);
			ourShader.setUniform("transform", trans);
			ourShader.setUniform("view", view);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			ourShader.setUniform("ourTexture1", 0);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			SwapBuffers(hDC);
			// ��������� ���������� =================================================================

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

	void mouseMove()
	{
		if (isFirstMouse)
		{
			lastX = mousePositionX;
			lastY = mousePositionY;

			isFirstMouse = false;
		}

		xpos = mousePositionX - lastX;
		ypos = lastY - mousePositionY;

		lastX = mousePositionX;
		lastY = mousePositionY;

		camera.ProcessMouseMovement(xpos, ypos);
	}

	void loadImage(GLuint &texture, char const* fileName)
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

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(image);
	}

	void Do_Movement()
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
	}
};
