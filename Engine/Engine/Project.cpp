#include "Project.h"

namespace Project
{
	void New(OPENFILENAME &ofn)
	{
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = WinApi::hWndEngine;
		ofn.lpstrFilter = "Файл проекта DarkMantiEngine (*.dmep)\0*.dmep\0Все файлы (*.*)\0*.*\0\0";
		ofn.lpstrFile = (LPSTR)WinApi::lastProjectFileName.c_str();
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = (LPCSTR)"dmep";
		ofn.lpstrTitle = "Создать проект";

		if (!GetSaveFileName(&ofn))
		{
			WinApi::Debug("Отмена");
		}

		WinApi::Debug("Создаю проект "); WinApi::Debug(ofn.lpstrFile);

		{
			char *tmp = ofn.lpstrFile;

			WinApi::lastProjectFileName = std::string(tmp);
		}

		std::ofstream file;
		file.open(ofn.lpstrFile);

		if (!file.is_open())
		{
			WinApi::Debug("Не удалось создать проект");
			return;
		}

		// Отбрасывание имени файла до корня проекта
		{
			char *c_str_tmp = ofn.lpstrFile;
			std::string string_tmp(c_str_tmp);

			WinApi::projectPath = string_tmp.substr(string_tmp.rfind('/'));
		}

		WinApi::Debug("Проект создан");
		file.close();
	}

	void Save(OPENFILENAME &ofn)
	{
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = WinApi::hWndEngine;
		ofn.lpstrFilter = "Файл проекта DarkMantiEngine (*.dmep)\0*.dmep\0Все файлы (*.*)\0*.*\0\0";
		ofn.lpstrFile = (LPSTR)WinApi::lastProjectFileName.c_str();
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = (LPCSTR)"dmep";
		ofn.lpstrTitle = "Сохранить проект";

		if (!GetSaveFileName(&ofn))
		{
			WinApi::Debug("Отмена");
			return;
		}

		WinApi::Debug("Сохраняю проект "); WinApi::Debug(ofn.lpstrFile);

		{
			char *tmp = ofn.lpstrFile;

			WinApi::lastProjectFileName = std::string(tmp);
		}

		std::ofstream file;

		file.open(WinApi::lastProjectFileName);

		if (!file.is_open())
		{
			WinApi::Debug("Не удалось сохранить файл проекта");
			return;
		}

		// Отбрасывание имени файла до корня проекта
		{
			char *c_str_tmp = ofn.lpstrFile;
			std::string string_tmp(c_str_tmp);

			WinApi::projectPath = string_tmp.substr(string_tmp.rfind('/'));
		}

		WinApi::Debug("Проект сохранен");
		file.close();
	}

	void Load(OPENFILENAME &ofn)
	{
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = WinApi::hWndEngine;
		ofn.lpstrFilter = "Файл проекта DarkMantiEngine (*.dmep)\0*.dmep\0Все файлы (*.*)\0*.*\0\0";
		ofn.lpstrFile = (LPSTR)WinApi::lastProjectFileName.c_str();
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = (LPCSTR)"dmep";
		ofn.lpstrTitle = "Открыть проект";

		if (!GetSaveFileName(&ofn))
		{
			WinApi::Debug("Отмена");
			return;
		}

		WinApi::Debug("Открываю проект "); WinApi::Debug(ofn.lpstrFile);

		{
			char *tmp = ofn.lpstrFile;

			WinApi::lastProjectFileName = std::string(tmp);
		}

		std::ifstream file;

		file.open(WinApi::lastProjectFileName);

		if (!file.is_open())
		{
			WinApi::Debug("Не удалось открыть файл проекта");
			return;
		}

		// Отбрасывание имени файла до корня проекта
		{
			char c_str_tmp[256];
			file >> c_str_tmp;
			std::string string_tmp(c_str_tmp);

			WinApi::projectPath = string_tmp.substr(string_tmp.rfind('/'));
		}

		WinApi::Debug("Проект открыт");
		file.close();
	}
}
