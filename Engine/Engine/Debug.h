#pragma once
#include <windows.h>

extern HANDLE			debugConsole;

void Debug(const char* sms);
void Debug(int str);
void Debug(unsigned int str);
void Debug(float str);
void Debug(long long str);

void Clear();

void GetFileSize(const char* filePath, LARGE_INTEGER* fileSize);
void ReadFileToBuffer(const char* filePath, LPVOID buffer, LARGE_INTEGER fileSize);