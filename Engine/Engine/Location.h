#pragma once

#include <vector>
#include <stdint.h>
#include <windows.h>

#include "GameObject.h"

extern std::vector<GameObject*> object_list;

void InitVarLoc();
void UninitVarLoc();

void NewLoc();
void SaveLoc();
void OpenLoc();

int8_t AddGameObject(const char *path);
int8_t RemoveGameObject();

int8_t AddScript();
int8_t RemoveScript();
