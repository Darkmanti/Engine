// ���� �������� � ���� ����� ����� ������������� �������� ����
#include "windows.h"
#include "stdint.h"

// �������� ������� ����� ��� ����������� �������� ��� ��������� � ��������� WNDCLASSA
LRESULT WndProc(HWND, uint32_t, WPARAM, LPARAM);

// ��������� ������ ����
WNDCLASSA g_pWndClassA;

// ���������� ����������
HINSTANCE hInstance;

// ���������� ����
HWND hWnd;

// ����������� ���� ��� ������������� ����������� �����������
ATOM RegisterEngineWindow(const HINSTANCE hInstance)
{
	// ��������� ���� ��������
	g_pWndClassA.style = CS_VREDRAW | CS_HREDRAW;			// ����� ����
	g_pWndClassA.lpfnWndProc = WndProc;						// ��������� �� ������� ���������
	g_pWndClassA.hInstance = hInstance;						// ���������� ����������
	g_pWndClassA.hCursor = LoadCursor(NULL, IDC_ARROW);		// ��������� ������
	g_pWndClassA.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);	// ��������� �� ����� � ������ ���� (���� ����� - ���������)
	g_pWndClassA.lpszClassName = "EngineWndClass\0";		// ������������ ������

	int16_t iError(RegisterClass(&g_pWndClassA));

	// �������� �� ���������� ����������� ����
	if (!iError)
	{
		MessageBox(NULL, "����� �� ���������������", "������", MB_OK);
		return 0;
	}

	return iError;
}

// �������� ����
uint8_t CreateEngineWindow(const int32_t nCmdShow)
{
	// ���������� ����
	hWnd = CreateWindow(
		g_pWndClassA.lpszClassName,				// �������� ������
		static_cast<LPCTSTR>("Engine\0"),		// �������� ����
		static_cast<DWORD>(g_pWndClassA.style), // ����� ����
		0, 0,									// �������
		640, 480,								// ������
		0,										// ��� �����-�� ������
		0,										// � ��� ���-�� ���
		hInstance,								// ���������� ����������
		0										// ��� ��� ����� �������� �� msdn
	);

	if (!hWnd)
	{
		MessageBox(hWnd, "������", "���� �� �������", MB_OK);
		return 0;
	}

	// �������� ����
	ShowWindow(hWnd, nCmdShow);

	// ��������� ����
	UpdateWindow(hWnd);

	return 1;
}

// ������� ����������� ��������� (��������� ����)
LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONUP: // ������� �� ���������
		MessageBox(hWnd, "�� ��������!", "�������", MB_OK);
		break;

	case WM_DESTROY: // ������� �� ���������
		PostQuitMessage(0);
		break;

	default: // ��� �������������� ��������� ���������� ���� Windows
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

// ����� � ������ ���������
void Loop()
{
	MSG message{ 0 }; 	// ��������� ��������� � ����
	int8_t iResult;		// ��� ���������

	// ���� ���� ���������
	while ((iResult = GetMessage(&message, NULL, 0, 0)))
	{
		// ���� ������� ������� ������������� ��� (������), �� ������� �� ����� ���������
		if (iResult < 0) break;

		// ����� ������������ ��������� � WndProc
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}
