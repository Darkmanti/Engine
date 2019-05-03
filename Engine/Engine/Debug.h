#pragma once
#include <windows.h>

extern HANDLE			debugConsole;

void Debug(const char *sms);
void Debug(int str);
void Debug(unsigned int str);
void Debug(float str);
void Clear();