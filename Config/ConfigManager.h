#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
using namespace std;

class ConfigManager {
public:
	static DWORD delay;
	static bool  antiMute;
	static vector<string> messages;

	ConfigManager();
	static void parseConfig();
private:
	static string path;
	static DWORD parseDelay(const char* str);
};