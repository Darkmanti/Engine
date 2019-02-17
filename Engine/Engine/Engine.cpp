#include "Engine.h"

#include <iostream>
#include <fstream>

#include "WinApi.h"

static int LoadDefaultSettings()
{
	WinApi::windowPositionX				= DEFAULT_WINDOW_POSITION_X;
	WinApi::windowPositionY				= DEFAULT_WINDOW_POSITION_Y;
	WinApi::windowWidth					= DEFAULT_WINDOW_WIDTH;
	WinApi::windowHeight				= DEFAULT_WINDOW_HEIGHT;

	WinApi::windowRenderPositionX		= DEFAULT_WINDOW_RENDER_POSITION_X;
	WinApi::windowRenderPositionY		= DEFAULT_WINDOW_RENDER_POSITION_Y;
	WinApi::windowRenderWidth			= DEFAULT_WINDOW_RENDER_WIDTH;
	WinApi::windowRenderHeight			= DEFAULT_WINDOW_RENDER_HEIGHT;

	WinApi::windowLocationPositionX		= DEFUALT_WINDOW_LOCATION_POSITION_X;
	WinApi::windowLocationPositionY		= DEFUALT_WINDOW_LOCATION_POSITION_Y;
	WinApi::windowLocationWidth			= DEFUALT_WINDOW_LOCATION_WIDTH;
	WinApi::windowLocationHeight		= DEFUALT_WINDOW_LOCATION_HEIGHT;

	WinApi::windowProjectPositionX		= DEFUALT_WINDOW_PROJECT_POSITION_X;
	WinApi::windowProjectPositionY		= DEFUALT_WINDOW_PROJECT_POSITION_Y;
	WinApi::windowProjectWidth			= DEFUALT_WINDOW_PROJECT_WIDTH;
	WinApi::windowProjectHeight			= DEFUALT_WINDOW_PROJECT_HEIGHT;

	WinApi::isFullscreen				= DEFAULT_IS_FULLSCREEN;
	WinApi::buttonsSize					= DEFAULT_BUTTON_SIZE;

	return 0;
}

namespace Engine
{
	HINSTANCE		hInstance;		// ���������� ����������

	std::string		dirAppData;		// ����������� ������ ����������

	Camera			*camera;

	bool			isLoaded;

	char			szDirect[MAX_PATH],
					szFileName[MAX_PATH];

	void NewLocation()
	{

	}

	void OpenLocation()
	{
		OPENFILENAME &ofn(WinApi::OFN);

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szDirect;
		*(ofn.lpstrFile) = 0;
		ofn.nMaxFile = sizeof(szDirect);
		ofn.lpstrFilter = NULL;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = szFileName;
		*(ofn.lpstrFileTitle) = 0;
		ofn.nMaxFileTitle = sizeof(szFileName);
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_EXPLORER;
		GetOpenFileName(&ofn);
	}

	void SaveLocation()
	{

	}

	void SaveAsLocation()
	{
		OPENFILENAME &ofn(WinApi::OFN);

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szDirect;
		*(ofn.lpstrFile) = 0;
		ofn.nMaxFile = sizeof(szDirect);
		ofn.lpstrFilter = NULL;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = szFileName;
		*(ofn.lpstrFileTitle) = 0;
		ofn.nMaxFileTitle = sizeof(szFileName);
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_EXPLORER;
		GetSaveFileName(&ofn);
	}

	// �������� �������
	void LoadConfigSettingsInterface()
	{
		std::fstream file;

		file.open(dirAppData + std::string("\\settings.cfg"), std::ios::in);

		if (!file.is_open())
		{
			// �������� ��������� ��������
			LoadDefaultSettings();

			return;
		}

		// ���� ������ �������� ����������
		try
		{
			std::string param;
			int32_t value;

			while (!file.eof())
			{
				file >> param;

				if (param == "//")
				{
					char cstr[64];
					file.getline(cstr, 64);
					continue;
				}

				if (param == "") continue;

				file >> value;

				// � ���������� ������
				if		(param == "windowPositionX")				WinApi::windowPositionX = value;
				else if (param == "windowPositionY")				WinApi::windowPositionY = value;
				else if (param == "windowWidth")					WinApi::windowWidth = value;
				else if (param == "windowHeight")					WinApi::windowHeight = value;

				else if (param == "windowRenderPositionX")			WinApi::windowRenderPositionX = value;
				else if (param == "windowRenderPositionY")			WinApi::windowRenderPositionY = value;
				else if (param == "windowRenderWidth")				WinApi::windowRenderWidth = value;
				else if (param == "windowRenderHeight")				WinApi::windowRenderHeight = value;

				else if (param == "windowLocationPositionX")		WinApi::windowLocationPositionX = value;
				else if (param == "windowLocationPositionY")		WinApi::windowLocationPositionY = value;
				else if (param == "windowLocationWidth")			WinApi::windowLocationWidth = value;
				else if (param == "windowLocationHeight")			WinApi::windowLocationHeight = value;
				
				else if (param == "windowProjectPositionX")			WinApi::windowProjectPositionX = value;
				else if (param == "windowProjectPositionY")			WinApi::windowProjectPositionY = value;
				else if (param == "windowProjectWidth")				WinApi::windowProjectWidth = value;
				else if (param == "windowProjectHeight")			WinApi::windowProjectHeight = value;

				else if (param == "isFullscreen")					WinApi::isFullscreen = value;
				else if (param == "buttonsSize")					WinApi::buttonsSize = value;
			}
		}
		catch (int) // ���� ����� ����������, ��
		{
			MessageBox(WinApi::hWndRender, "���� ����� ������������. ����� � ����������� ����������", "������", MB_OK);
			LoadDefaultSettings();
		}

		file.close();
	}

	// ���������� �������
	void SaveConfigSettingsInterface()
	{
		std::ofstream file;

		file.open(dirAppData + std::string("\\settings.cfg"), std::ios::out);

		if (!file.is_open())
		{
			return;
		}

		file << "// File config" << '\n';

		file << "windowPositionX "				<< WinApi::windowPositionX << '\n';
		file << "windowPositionY "				<< WinApi::windowPositionY << '\n';
		file << "windowWidth "					<< WinApi::windowWidth << '\n';
		file << "windowHeight "					<< WinApi::windowHeight << '\n';

		file << "windowRenderPositionX "		<< WinApi::windowRenderPositionX << '\n';
		file << "windowRenderPositionY "		<< WinApi::windowRenderPositionY << '\n';
		file << "windowRenderWidth "			<< WinApi::windowRenderWidth << '\n';
		file << "windowRenderHeight "			<< WinApi::windowRenderHeight << '\n';

		file << "windowLocationPositionX "		<< WinApi::windowLocationPositionX << '\n';
		file << "windowLocationPositionY "		<< WinApi::windowLocationPositionY<< '\n';
		file << "windowLocationWidth "			<< WinApi::windowLocationWidth << '\n';
		file << "windowLocationHeight "			<< WinApi::windowLocationHeight << '\n';

		file << "windowProjectPositionX "		<< WinApi::windowProjectPositionX << '\n';
		file << "windowProjectPositionY "		<< WinApi::windowProjectPositionY << '\n';
		file << "windowProjectWidth "			<< WinApi::windowProjectWidth << '\n';
		file << "windowProjectHeight "			<< WinApi::windowProjectHeight << '\n';

		file << "isFullscreen "					<< WinApi::isFullscreen << '\n';
		file << "buttonsSize "					<< WinApi::buttonsSize << '\n';

		file.close();
	}
};
