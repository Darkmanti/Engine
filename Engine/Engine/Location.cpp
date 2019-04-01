#include "Location.h"

#include <fstream>

#include "WinApi.h"
//#include "GameObject.h"

namespace Location
{
	void New(OPENFILENAME &ofn)
	{
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = WinApi::hWndEngine;
		ofn.lpstrFilter = "���� ������� DarkMantiEngine (*.dmel)\0*.dmel\0��� ����� (*.*)\0*.*\0\0";
		ofn.lpstrFile = (LPSTR)WinApi::lastLocationFileName.c_str();
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = (LPCSTR)"dmel";
		ofn.lpstrTitle = "������� �������";

		GetSaveFileName(&ofn);
		WinApi::Debug("������ ������� "); WinApi::Debug(ofn.lpstrFile);

		{
			char *tmp = ofn.lpstrFile;

			WinApi::lastProjectFileName = std::string(tmp);
		}

		std::ofstream file;

		file.open(ofn.lpstrFile);

		file << "";

		WinApi::Debug("������� �������");
		file.close();
	}

	void Save(OPENFILENAME &ofn)
	{
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = WinApi::hWndEngine;
		ofn.lpstrFilter = "���� ������� DarkMantiEngine (*.dmel)\0*.dmel\0��� ����� (*.*)\0*.*\0\0";
		ofn.lpstrFile = (LPSTR)WinApi::lastLocationFileName.c_str();
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = (LPCSTR)"dmel";
		ofn.lpstrTitle = "��������� �������";

		if (!GetSaveFileName(&ofn))
		{
			WinApi::Debug("�� ������� ������� �������");
		}

		WinApi::Debug("�������� ������� "); WinApi::Debug(ofn.lpstrFile);

		{
			char *tmp = ofn.lpstrFile;

			WinApi::lastProjectFileName = std::string(tmp);
		}

		std::ofstream file;

		file.open(ofn.lpstrFile);

		//for (int i(0); i < WinApi::countGameObjects; ++i)
		//{
		//	file << GameObject::fileName << "\n";
		//}

		WinApi::Debug("������� ���������");
		file.close();
	}

	void Load()
	{

	}
}
