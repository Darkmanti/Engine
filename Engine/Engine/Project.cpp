#include "Project.h"

namespace Project
{
	void New(OPENFILENAME &ofn)
	{
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = WinApi::hWndEngine;
		ofn.lpstrFilter = "���� ������� DarkMantiEngine (*.dmep)\0*.dmep\0��� ����� (*.*)\0*.*\0\0";
		ofn.lpstrFile = (LPSTR)WinApi::lastProjectFileName.c_str();
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = (LPCSTR)"dmep";
		ofn.lpstrTitle = "������� ������";

		if (!GetSaveFileName(&ofn))
		{
			WinApi::Debug("������");
		}

		WinApi::Debug("������ ������ "); WinApi::Debug(ofn.lpstrFile);

		{
			char *tmp = ofn.lpstrFile;

			WinApi::lastProjectFileName = std::string(tmp);
		}

		std::ofstream file;
		file.open(ofn.lpstrFile);

		if (!file.is_open())
		{
			WinApi::Debug("�� ������� ������� ������");
			return;
		}

		// ������������ ����� ����� �� ����� �������
		{
			char *c_str_tmp = ofn.lpstrFile;
			std::string string_tmp(c_str_tmp);

			WinApi::projectPath = string_tmp.substr(string_tmp.rfind('/'));
		}

		WinApi::Debug("������ ������");
		file.close();
	}

	void Save(OPENFILENAME &ofn)
	{
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = WinApi::hWndEngine;
		ofn.lpstrFilter = "���� ������� DarkMantiEngine (*.dmep)\0*.dmep\0��� ����� (*.*)\0*.*\0\0";
		ofn.lpstrFile = (LPSTR)WinApi::lastProjectFileName.c_str();
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = (LPCSTR)"dmep";
		ofn.lpstrTitle = "��������� ������";

		if (!GetSaveFileName(&ofn))
		{
			WinApi::Debug("������");
			return;
		}

		WinApi::Debug("�������� ������ "); WinApi::Debug(ofn.lpstrFile);

		{
			char *tmp = ofn.lpstrFile;

			WinApi::lastProjectFileName = std::string(tmp);
		}

		std::ofstream file;

		file.open(WinApi::lastProjectFileName);

		if (!file.is_open())
		{
			WinApi::Debug("�� ������� ��������� ���� �������");
			return;
		}

		// ������������ ����� ����� �� ����� �������
		{
			char *c_str_tmp = ofn.lpstrFile;
			std::string string_tmp(c_str_tmp);

			WinApi::projectPath = string_tmp.substr(string_tmp.rfind('/'));
		}

		WinApi::Debug("������ ��������");
		file.close();
	}

	void Load(OPENFILENAME &ofn)
	{
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = WinApi::hWndEngine;
		ofn.lpstrFilter = "���� ������� DarkMantiEngine (*.dmep)\0*.dmep\0��� ����� (*.*)\0*.*\0\0";
		ofn.lpstrFile = (LPSTR)WinApi::lastProjectFileName.c_str();
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = (LPCSTR)"dmep";
		ofn.lpstrTitle = "������� ������";

		if (!GetSaveFileName(&ofn))
		{
			WinApi::Debug("������");
			return;
		}

		WinApi::Debug("�������� ������ "); WinApi::Debug(ofn.lpstrFile);

		{
			char *tmp = ofn.lpstrFile;

			WinApi::lastProjectFileName = std::string(tmp);
		}

		std::ifstream file;

		file.open(WinApi::lastProjectFileName);

		if (!file.is_open())
		{
			WinApi::Debug("�� ������� ������� ���� �������");
			return;
		}

		// ������������ ����� ����� �� ����� �������
		{
			char c_str_tmp[256];
			file >> c_str_tmp;
			std::string string_tmp(c_str_tmp);

			WinApi::projectPath = string_tmp.substr(string_tmp.rfind('/'));
		}

		WinApi::Debug("������ ������");
		file.close();
	}
}
