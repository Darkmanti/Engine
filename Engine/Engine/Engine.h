#pragma once

#include "GLEW/glew.h"

#include <windows.h>
#include <stdint.h>
#include <string>

// ��������� ������, winapi � ������ �����
namespace Engine
{
	extern HINSTANCE hInstance;			// ���������� ����������

	extern std::string dirAppData;		// ����������� ������ ����������

	// �������� �������
	void LoadConfigSettingsInterface();

	// ���������� �������
	void SaveConfigSettingsInterface();
};
