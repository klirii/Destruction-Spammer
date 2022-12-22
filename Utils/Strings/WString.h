#pragma once
#include <iostream>
#include <Windows.h>
using namespace std;

class WString {
public:
	static wstring GetSymbolFromCString(const char* str, USHORT index);
	static wstring GetWStringFrom—String(const char* str);
};