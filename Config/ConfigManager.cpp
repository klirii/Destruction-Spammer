#define _CRT_SECURE_NO_WARNINGS
#include "ConfigManager.h"
#include "../Utils/Strings/String.h"
#include <fstream>

#include <StringUtils.h>
#include <StringUtils.cpp>

string ConfigManager::path = string(getenv("APPDATA")) + "\\.vimeworld\\minigames\\Spammer.ini";

bool ConfigManager::antiMute = true;
DWORD ConfigManager::delay = 0;
string ConfigManager::keybind;
vector<string> ConfigManager::messages;

ConfigManager::ConfigManager() {
	struct _stat fiBuf;
	if (_stat(ConfigManager::path.c_str(), &fiBuf) == -1) {
		ofstream config(ConfigManager::path);
		config << "keybind=F10\n";
		config << "delay=20s\n";
		config << "antimute=true\n";
		config << "message=Destruction Spammer Ч лучший спамер дл€ VimeWorld!";
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
	char line[108];
	string strLine;

	char** lineParts = new char* [2];
	string tmpLinePart;

	ifstream config(ConfigManager::path);
	while (config.getline(line, 108)) {
		strLine = line;
		if (strLine.find("keybind") != string::npos) {
			StringUtils::split(line, '=', lineParts);
			ConfigManager::keybind = String(lineParts[1]).toUpper();
		}
		else if (strLine.find("delay") != string::npos) {
			StringUtils::split(line, '=', lineParts);
			ConfigManager::delay = ConfigManager::parseDelay(String(lineParts[1]).toLower().c_str());
		}
		else if (strLine.find("antimute") != string::npos) {
			StringUtils::split(line, '=', lineParts);
			if (strcmp(String(lineParts[1]).toLower().c_str(), "true") == 0) ConfigManager::antiMute = true;
			else if (strcmp(String(lineParts[1]).toLower().c_str(), "false") == 0) ConfigManager::antiMute = false;
		}
		else if (strLine.find("message") != string::npos) {
			if (ConfigManager::messages.size() == 255) break;
			StringUtils::split(line, '=', lineParts);
			ConfigManager::messages.push_back(lineParts[1]);
		}
	}

	StringUtils::remove(lineParts, 2);
}