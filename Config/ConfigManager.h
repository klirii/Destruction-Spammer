#pragma once
#include <iostream>
#include <Windows.h>
using namespace std;

class ConfigManager {
public:
	static DWORD  delay;
	static string message;

	ConfigManager();
	static void parseConfig();
private:
	static string path;
	static DWORD parseDelay(const char* str);
};