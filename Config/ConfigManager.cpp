#define _CRT_SECURE_NO_WARNINGS

#include "ConfigManager.h"

#include "../Utils/Strings/String.h"
#include "../Utils/Strings/Conversions.hpp"

#include <fstream>
#include <codecvt>
#include <string>

#include <StringUtils.h>
#include <StringUtils.cpp>

std::string ConfigManager::loaderPath = "";
std::string ConfigManager::gamePath = "";
string ConfigManager::path = "";

bool ConfigManager::antiMute = true;
DWORD ConfigManager::delay = 0;
string ConfigManager::keybind;
vector<string> ConfigManager::messages;

string ConfigManager::parseUsername(bool game) {
	if (!game) {
		loaderPath = string(getenv("APPDATA")) + "\\.vimeworld\\jre-x64\\lib\\security\\java8.security";
		wstring username;

		wifstream config(loaderPath);
		config.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
		getline(config, username);

		config.close();
		if (string(username.begin(), username.end()).empty()) return "";
		return string(username.begin(), username.end());
	}
	else {
		gamePath = string(getenv("APPDATA")) + "\\.vimeworld\\config";

		wstring username;
		wifstream config(gamePath);

		config.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
		for (uint8_t i = 0; i < 2; i++) getline(config, username);

		char* lineParts[2];
		StringUtils::split(string(username.begin(), username.end()).c_str(), ':', lineParts);

		config.close();
		if (string(lineParts[1]).empty()) return "";
		return string(lineParts[1]);
	}
}

string ConfigManager::parsePassword() {
	ifstream config(loaderPath);
	string password;

	for (uint8_t i = 0; i < 2; i++)
		getline(config, password);

	return password;
}

ConfigManager::ConfigManager() {
	path = string(getenv("APPDATA")) + "\\.vimeworld\\minigames\\Spammer.ini";

	struct _stat fiBuf;
	if (_stat(ConfigManager::path.c_str(), &fiBuf) == -1) {
		wofstream config(ConfigManager::path, ios::binary);
		config.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

		cout << "pidor" << endl;
		config << L"keybind=F10\n";
		config << L"delay=20s\n";
		config << L"antimute=true\n";
		config << L"message=Destruction Spammer - лучший спамер для VimeWorld!";
		config.close();
	}
}

DWORD ConfigManager::parseDelay(const char* str) {
	const char* units[]		= {"ms", "s", "m", "h"};
	DWORD unitMultipliers[] = {1, 1000, 60000, 3600000};

	for (byte i = 0; str[i]; i++) {
		for (byte j = 0; j < 4; j++) {
			if (str[i] == (*units[j])) {
				if (strcmp(str + i, units[j]) != 0) continue;

				USHORT strLen = StringUtils::len(str);
				char* delay = new char[strLen];
				StringUtils::cpy(delay, str, StringUtils::len(str));
#				pragma warning(suppress:6011)
				delay[i] = '\0';

				DWORD ms = atoi(delay) * unitMultipliers[j];
				delete[] delay;

				if (ms > 86400000) return NULL;
				return ms;
			}
		}
	}

	return NULL;
}

void ConfigManager::parseConfig() {
	string line;
	wstring wline;
	char* lineParts[2];

	wifstream config(ConfigManager::path);
	config.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));

	while (getline(config, wline)) {
		line = unicode2ansi(wline);
		if (line.find("keybind") != string::npos) {
			StringUtils::split(line.c_str(), '=', lineParts);
			ConfigManager::keybind = String(lineParts[1]).toUpper();
		}
		else if (line.find("delay") != string::npos) {
			StringUtils::split(line.c_str(), '=', lineParts);
			ConfigManager::delay = ConfigManager::parseDelay(String(lineParts[1]).toLower().c_str());
		}
		else if (line.find("antimute") != string::npos) {
			StringUtils::split(line.c_str(), '=', lineParts);
			if (strcmp(String(lineParts[1]).toLower().c_str(), "true") == 0) ConfigManager::antiMute = true;
			else if (strcmp(String(lineParts[1]).toLower().c_str(), "false") == 0) ConfigManager::antiMute = false;
		}
		else if (line.find("message") != string::npos) {
			if (ConfigManager::messages.size() == 255) break;
			if (line.length() > 108) line[108] = '\0';

			StringUtils::split(line.c_str(), '=', lineParts);
			ConfigManager::messages.push_back(lineParts[1]);
		}
	}
}