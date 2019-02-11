#pragma once

#include "GLEW/glew.h"

#include <windows.h>
#include <stdint.h>
#include <string>

#include "Camera.h"

// ��������� ������, winapi � ������ �����
namespace Engine
{
	extern HINSTANCE hInstance;			// ���������� ����������

	extern std::string dirAppData;		// ����������� ������ ����������

	extern Camera *camera;

	extern bool isLoaded;

	// �������� �������
	void LoadConfigSettingsInterface();

	// ���������� �������
	void SaveConfigSettingsInterface();
};
