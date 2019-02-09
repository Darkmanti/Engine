#include "Graphics.h"

namespace Graphics
{
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

	void CreateObject(GLfloat vertices[], GLuint VAO, GLuint VBO)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	void DrawObject(GLuint VAO)
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void DisableOpenGL()
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(WinApi::hRC);
		ReleaseDC(WinApi::hWndRender, WinApi::hDC);
	}
}
