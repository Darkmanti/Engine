#include "Interface.h"
#include "Location.h"

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

	for (int i = 0; i < object_list.size(); i++)
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

	for (int i = 0; i < (int)object_list.size(); i++)
	{
		// ���������:
		// "delete"
	}

	// ���������� out.exe

	return 0;
}