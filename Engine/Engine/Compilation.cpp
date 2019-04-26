#include "WinApi.h"

int Compilation()
{
	// Создание папки build

	// #include GameObject.h
	// #include Script.h
	// #include GameObject.cpp
	// #include Script.cpp
	// #include все скрипты в out.cpp

	// Вставить:
	// GameObject *objects = new GameObject[gameobject_count];

	for (int i = 0; i < Location::gameobject_count; i++)
	{
		// Вставляем:
		// "object[" + i + "] = new GameObject(Location::...);\n"
	}

	// Вставляем:
	/*
	for i in objects
	{
		for j in i.scripts
		{
			i.j.start();
		}
	}
	*/

	// Вставляем:
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
		// Вставляем:
		// "delete"
	}

	// Компиляция out.exe

	return 0;
}