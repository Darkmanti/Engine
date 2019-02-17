#include "Graphics.h"

namespace Graphics
{
	GLfloat lastFrame,
			deltaTime;

	void EnableOpenGL()
	{
		PIXELFORMATDESCRIPTOR pfd;

		int iFormat;

		WinApi::hDC = GetDC(WinApi::hWndRender);

		ZeroMemory(&pfd, sizeof(pfd));

		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 16;
		pfd.iLayerType = PFD_MAIN_PLANE;
		iFormat = ChoosePixelFormat(WinApi::hDC, &pfd);
		SetPixelFormat(WinApi::hDC, iFormat, &pfd);

		WinApi::hRC = wglCreateContext(WinApi::hDC);
		wglMakeCurrent(WinApi::hDC, WinApi::hRC);
	}

	void DisableOpenGL()
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(WinApi::hRC);
		ReleaseDC(WinApi::hWndRender, WinApi::hDC);
	}
}
