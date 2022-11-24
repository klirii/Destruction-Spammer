#include <Windows.h>
#include <iostream>

#include "JNI/Handler.h"
#include "Config/ConfigManager.h"
#include "Core/Spammer.h"
#include "JNI/Classes/net/xtrafrancyz/util/CommonUtils.h"
using namespace std;

HANDLE hOnDelay = nullptr;

void onDelay() {
	JNIHandler::setEnv();
	JNIHandler::setClassLoader();

	while (true) {
		ConfigManager::parseConfig();
		wstring formattedMessage = Spammer::getFormattedMessage(ConfigManager::message.c_str());
		CommonUtils::sendMessage(JNIHandler::env->NewString(reinterpret_cast<const jchar*>(formattedMessage.c_str()), static_cast<jsize>(formattedMessage.size())));
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