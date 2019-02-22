#include "WinApi.h"

#include <CommCtrl.h>
#include <algorithm>
#include <thread>
#include <windowsx.h>

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
		pWndEngineClassEx.hInstance = hInstance;							// ���������� ����������
		pWndEngineClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);					// ��������� ������
		pWndEngineClassEx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);				// ��������� �� ����� � ������ ���� (���� ����� - ���������)
		pWndEngineClassEx.lpszClassName = "WndEngineClass";							// ������������ ������
		pWndEngineClassEx.hIcon = LoadIcon(hInstance, "IDI_ENGINEICON");	// ������

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
		pWndRenderClassEx.style = CS_VREDRAW | CS_HREDRAW | CS_BYTEALIGNCLIENT;					// ����� ����
		pWndRenderClassEx.lpfnWndProc = WndRenderProc;						// ��������� �� ������� ���������
		pWndRenderClassEx.hInstance = hInstance;					// ���������� ����������
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

	// �������� ����
	uint16_t CreateWindowEngine()
	{
		// ���������� ����
		hWndEngine = CreateWindowEx(WS_EX_ACCEPTFILES,									// Extended style
			pWndEngineClassEx.lpszClassName,											// �������� ������
			"������",																	// �������� ����
			WS_OVERLAPPEDWINDOW,														// ����� ����
			0, 0,											// �������
			1366, 768,													// ������
			0,																			// ������������ ����
			0,																			// ����
			hInstance,															// ���������� ����������
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
			256, 256,	// �������
			800, 600,			// ������
			hWndEngine,										// ������������ ����
			0,												// ����
			hInstance,								// ���������� ����������
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
		if (ShowWindow(hWndEngine, nCmdShow)) return 2;
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

	// ����� � ������ ���������
	void Loop()
	{
		MSG message{ 0 }; 	// ��������� ��������� � ����
		int8_t iResult;		// ��� ���������

		// ���� ���� ���������
		// ���� ������� ������� ������������� ��� (������), �� ������� �� ����� ���������
		while ((iResult = GetMessage(&message, NULL, 0, 0)))
		{
			// ������������ ��������� � WndProc
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
