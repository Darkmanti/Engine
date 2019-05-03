#include "Debug.h"
#include <sstream>

HANDLE			debugConsole;

void Debug(const char *sms)
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

void Clear()
{
	if (debugConsole == nullptr)
		return;

	system("cls");
}