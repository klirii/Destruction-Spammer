#define _CRT_SECURE_NO_WARNINGS
#include "WString.h"

wstring WString::GetSymbolFromCString(const char* str, USHORT index) {
	byte messageLen = static_cast<byte>(strlen(str));
	char* messageDup = new char[messageLen + 1];
	strncpy(messageDup, str + index, (messageLen - index) + 1);

	messageDup[1] = '\0';
	wstring ws(1, L'\0');
	mbstowcs(&ws[0], messageDup, 1);

	delete[] messageDup;
	return ws;
}