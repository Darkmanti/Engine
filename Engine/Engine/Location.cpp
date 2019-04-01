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
		ofn.lpstrFilter = "Файл локации DarkMantiEngine (*.dmel)\0*.dmel\0Все файлы (*.*)\0*.*\0\0";
		ofn.lpstrFile = (LPSTR)WinApi::lastLocationFileName.c_str();
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = (LPCSTR)"dmel";
		ofn.lpstrTitle = "Создать локацию";

		GetSaveFileName(&ofn);
		WinApi::Debug("Создаю локацию "); WinApi::Debug(ofn.lpstrFile);

		{
			char *tmp = ofn.lpstrFile;

			WinApi::lastProjectFileName = std::string(tmp);
		}

		std::ofstream file;

		file.open(ofn.lpstrFile);

		file << "";

		WinApi::Debug("Локация создана");
		file.close();
	}

	void Save(OPENFILENAME &ofn)
	{
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = WinApi::hWndEngine;
		ofn.lpstrFilter = "Файл локации DarkMantiEngine (*.dmel)\0*.dmel\0Все файлы (*.*)\0*.*\0\0";
		ofn.lpstrFile = (LPSTR)WinApi::lastLocationFileName.c_str();
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = (LPCSTR)"dmel";
		ofn.lpstrTitle = "Сохранить локацию";

		if (!GetSaveFileName(&ofn))
		{
			WinApi::Debug("Не удалось создать локацию");
		}

		WinApi::Debug("Сохраняю локацию "); WinApi::Debug(ofn.lpstrFile);

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

		WinApi::Debug("Локация сохранена");
		file.close();
	}

	void Load()
	{

	}
}
