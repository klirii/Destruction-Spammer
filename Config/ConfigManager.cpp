#define _CRT_SECURE_NO_WARNINGS
#include "ConfigManager.h"
#include <fstream>

#include <StringUtils.h>
#include <StringUtils.cpp>

string ConfigManager::path = string(getenv("APPDATA")) + "\\.vimeworld\\minigames\\Spammer.ini";

DWORD  ConfigManager::delay = 0;
string ConfigManager::message;

ConfigManager::ConfigManager() {
	struct _stat fiBuf;
	if (_stat(ConfigManager::path.c_str(), &fiBuf) == -1) {
		ofstream config(ConfigManager::path);
		config << "delay=20s\n";
		config << "message=Destruction Spammer — ëó÷øèé ñïàìåð äëÿ VimeWorld!";
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
				if (ms > 86400000) return NULL;
				delete[] delay;
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
	
	ifstream config(ConfigManager::path);
	while (config.getline(line, 108)) {
		strLine = line;
		if (strLine.find("delay") != string::npos) {
			StringUtils::split(line, '=', lineParts);
			ConfigManager::delay = ConfigManager::parseDelay(lineParts[1]);
		}
		else if (strLine.find("message") != string::npos) {
			StringUtils::split(line, '=', lineParts);
			ConfigManager::message = lineParts[1];
		}
	}

	StringUtils::remove(lineParts, 2);
}
