#include "WinApi.h"

int Compilation()
{
	// �������� ����� build

	// #include GameObject.h
	// #include Script.h
	// #include GameObject.cpp
	// #include Script.cpp
	// #include ��� ������� � out.cpp

	// ��������:
	// GameObject *objects = new GameObject[gameobject_count];

	for (int i = 0; i < Location::gameobject_count; i++)
	{
		// ���������:
		// "object[" + i + "] = new GameObject(Location::...);\n"
	}

	// ���������:
	/*
	for i in objects
	{
		for j in i.scripts
		{
			i.j.start();
		}
	}
	*/

	// ���������:
	/*
	while (1)
	{
		for i in objects
		{
			for j in i.scripts
			{
				i.j.update();
			}

			i.render();
		}
	}
	*/

	for (int i = 0; i < Location::gameobject_count; i++)
	{
		// ���������:
		// "delete"
	}

	// ���������� out.exe

	return 0;
}