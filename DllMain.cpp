#include <Windows.h>
#include <iostream>

#include "JNI/Handler.h"
#include "Config/ConfigManager.h"
#include "Core/Spammer.h"
#include "Utils/WString.h"

#include "JNI/Classes/net/xtrafrancyz/util/CommonUtils.h"
#include "JNI/Classes/net/xtrafrancyz/mods/texteria/Texteria.h"
using namespace std;

HANDLE hOnDelay = nullptr;
void onDelay() {
	JNIHandler::setEnv();
	JNIHandler::setClassLoader();
	
	UCHAR messageCounter = 0;
	while (true) {
		string serverId = Texteria::tryGetServerId("null");
		if (serverId == "null") {
			Sleep(ConfigManager::delay);
			continue;
		}
		if (serverId.find("'") != string::npos && serverId.find("LOBBY") != string::npos) {
			Sleep(ConfigManager::delay);
			continue;
		}

		ConfigManager::parseConfig();
		wstring message;

		if (ConfigManager::antiMute) message = Spammer::getFormattedMessage(ConfigManager::messages[messageCounter].c_str());
		else message = WString::GetWStringFromÑString(ConfigManager::messages[messageCounter].c_str());
		CommonUtils::sendMessage(JNIHandler::env->NewString(reinterpret_cast<const jchar*>(message.c_str()), static_cast<jsize>(message.size())));

		messageCounter++;
		if (messageCounter == ConfigManager::messages.size()) messageCounter = 0;
		Sleep(ConfigManager::delay);
	}
}

void init() {
	setlocale(LC_ALL, "ru");
	JNIHandler::setVM();

	ConfigManager::ConfigManager();
	ConfigManager::parseConfig();

	if (ConfigManager::delay != NULL)
		hOnDelay = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(onDelay), nullptr, NULL, nullptr);
}

BOOL APIENTRY DllMain(HINSTANCE handle, DWORD reason, LPVOID lpReserved) {
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(init), nullptr, NULL, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		if (hOnDelay != nullptr) CloseHandle(hOnDelay);
		break;
	}

	return TRUE;
}