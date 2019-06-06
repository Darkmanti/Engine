#include "Debug.h"
#include <sstream>

HANDLE			debugConsole;

void Debug(const char* sms)
{
	if (debugConsole == nullptr)
		return;

	WriteConsole(debugConsole, sms, strlen(sms), nullptr, NULL);
}

void Debug(int str)
{
	if (debugConsole == nullptr)
		return;

	char sms[255];

	_itoa(str, sms, 10);

	WriteConsole(debugConsole, sms, strlen(sms), nullptr, NULL);
}

void Debug(unsigned int str)
{
	if (debugConsole == nullptr)
		return;

	char sms[255];

	_itoa(str, sms, 10);

	WriteConsole(debugConsole, sms, strlen(sms), nullptr, NULL);
}

void Debug(float str)
{
	if (debugConsole == nullptr)
		return;

	char sms[255];
	std::stringstream iostr;

	iostr << str;
	iostr >> sms;

	WriteConsole(debugConsole, sms, strlen(sms), nullptr, NULL);
}

void Debug(long long str)
{
	char sms[255];

	_itoa(str, sms, 10);

	WriteConsole(debugConsole, sms, strlen(sms), nullptr, NULL);
}

void Clear()
{
	if (debugConsole == nullptr)
		return;

	system("cls");
}

void GetFileSize(const char* filePath, LARGE_INTEGER* fileSize)
{
	HANDLE fileHandle = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	GetFileSizeEx(fileHandle, fileSize);

	CloseHandle(fileHandle);
}

void ReadFileToBuffer(const char* filePath, LPVOID buffer, LARGE_INTEGER fileSize)
{
	HANDLE fileHandle = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	DWORD read;
	ReadFile(fileHandle, buffer, fileSize.QuadPart, &read, 0);

	CloseHandle(fileHandle);
}