#pragma once

#include <stdint.h>
#include <windows.h>
#include <Commdlg.h>
#include <Commctrl.h>
#pragma comment(lib,"Comctl32.Lib")

extern int32_t gameobject_count;
extern GameObject* object_list;

void New(OPENFILENAME &ofn);
void Save(OPENFILENAME &ofn);
void Load();

int8_t AddGameObject();
int8_t RemoveGameObject();

int8_t AddScript();
int8_t RemoveScript();
