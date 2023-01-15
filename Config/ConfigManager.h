#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
using namespace std;

class ConfigManager {
public:
	static bool antiMute;
	static DWORD delay;
	static string keybind;
	static vector<string> messages;

	ConfigManager();
	static void parseConfig();
	static string parseUsername(bool game = false);
private:
	static string path;
	static string gamePath;
	static string loaderPath;

	static DWORD parseDelay(const char* str);
};