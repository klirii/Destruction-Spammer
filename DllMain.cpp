#define _CRT_SECURE_NO_WARNINGS
#define DLL_VIMEWORLD_ATTACH 0x888

#include <Windows.h>
#include <iostream>

#include "JNI/Handler.h"
#include "Config/ConfigManager.h"

#include "Utils/Strings/WString.h"
#include "Utils/Hashes.hpp"

#include "Core/Spammer.h"
#include "Core/Settings/Keycodes.h"

#include "JNI/Classes/net/xtrafrancyz/util/CommonUtils.h"
#include "JNI/Classes/net/xtrafrancyz/mods/texteria/Texteria.h"

#include "RestAPI/Core/Client.hpp"

using namespace std;

static RestAPI::Client client;

void checkKeybind(LPVOID isEnabled) {
	map<string, byte> Keycodes = {
		{"MBUTTON", VK_MBUTTON},
		{"XBUTTON1", VK_XBUTTON1},
		{"XBUTTON2", VK_XBUTTON2},

		{"SHIFT", VK_SHIFT},
		{"LSHIFT", VK_LSHIFT},
		{"RSHIFT", VK_RSHIFT},

		{"CTRL", VK_CONTROL},
		{"LCTRL", VK_LCONTROL},
		{"RCTRL", VK_RCONTROL},

		{"ALT", VK_MENU},
		{"LALT", VK_LMENU},
		{"RALT", VK_RMENU},

		{"TAB", VK_TAB},
		{"BACKSPACE", VK_BACK},
		{"ENTER", VK_RETURN},
		{"PAUSE", VK_PAUSE},
		{"CAPS", VK_CAPITAL},
		{"ESCAPE", VK_ESCAPE},
		{"SPACE", VK_SPACE},
		{"PAGEUP", VK_PRIOR},
		{"PAGEDOWN", VK_NEXT},
		{"END", VK_END},
		{"HOME", VK_HOME},
		{"SELECT", VK_SELECT},
		{"PRINTSCREEN", VK_PRINT},
		{"NUM_LOCK", VK_NUMLOCK},
		{"SCROLL_LOCK", VK_SCROLL},

		{"0", 0x30},
		{"1", 0x31},
		{"2", 0x32},
		{"3", 0x33},
		{"4", 0x34},
		{"5", 0x35},
		{"6", 0x36},
		{"7", 0x37},
		{"8", 0x38},
		{"9", 0x39},

		{"NUMPAD_0", VK_NUMPAD0},
		{"NUMPAD_1", VK_NUMPAD1},
		{"NUMPAD_2", VK_NUMPAD2},
		{"NUMPAD_3", VK_NUMPAD3},
		{"NUMPAD_4", VK_NUMPAD4},
		{"NUMPAD_5", VK_NUMPAD5},
		{"NUMPAD_6", VK_NUMPAD6},
		{"NUMPAD_7", VK_NUMPAD7},
		{"NUMPAD_8", VK_NUMPAD8},
		{"NUMPAD_9", VK_NUMPAD9},

		{"F1", VK_F1},
		{"F2", VK_F2},
		{"F3", VK_F3},
		{"F4", VK_F4},
		{"F5", VK_F5},
		{"F6", VK_F6},
		{"F7", VK_F7},
		{"F8", VK_F8},
		{"F9", VK_F9},
		{"F10", VK_F10},
		{"F11", VK_F11},
		{"F12", VK_F12},

		{"A", 0x41},
		{"B", 0x42},
		{"C", 0x43},
		{"D", 0x44},
		{"E", 0x45},
		{"F", 0x46},
		{"G", 0x47},
		{"H", 0x48},
		{"I", 0x49},
		{"J", 0x4A},
		{"K", 0x4B},
		{"L", 0x4C},
		{"M", 0x4D},
		{"N", 0x4E},
		{"O", 0x4F},
		{"P", 0x50},
		{"Q", 0x51},
		{"R", 0x52},
		{"S", 0x53},
		{"T", 0x54},
		{"U", 0x55},
		{"V", 0x56},
		{"W", 0x57},
		{"X", 0x58},
		{"Y", 0x59},
		{"Z", 0x5A},

		{"LEFT", VK_LEFT},
		{"UP", VK_UP},
		{"RIGHT", VK_RIGHT},
		{"DOWN", VK_DOWN},
	};

	while (true) {
		if (Keycodes.count(ConfigManager::keybind)) {
			if (GetAsyncKeyState(Keycodes[ConfigManager::keybind]) & 1) {
				char title[128];
				GetWindowTextA(GetForegroundWindow(), title, 128);

				if (strcmp(title, "VimeWorld") == 0)
					*(bool*)isEnabled = !(*(bool*)isEnabled);
			}
		}
	}
}

HANDLE hOnDelay = nullptr;
void onDelay() {
	JNIHandler::setEnv();
	JNIHandler::setClassLoader();

	bool isEnabled = false;
	UCHAR messageCounter = 0;

	CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(checkKeybind), &isEnabled, NULL, nullptr);
	while (true) {
		ConfigManager::messages.clear();
		ConfigManager::parseConfig();

		if (isEnabled) {
			string serverId = Texteria::tryGetServerId("null");
			if (serverId == "null") {
				Sleep(ConfigManager::delay);
				continue;
			}
			if (serverId.find("'") != string::npos && serverId.find("LOBBY") != string::npos) {
				Sleep(ConfigManager::delay);
				continue;
			}

			wstring message;
			if (ConfigManager::antiMute) message = Spammer::getFormattedMessage(ConfigManager::messages[messageCounter].c_str());
			else message = WString::GetWStringFromÑString(ConfigManager::messages[messageCounter].c_str());
			CommonUtils::sendMessage(JNIHandler::env->NewString(reinterpret_cast<const jchar*>(message.c_str()), static_cast<jsize>(message.size())));

			messageCounter++;
			if (messageCounter == ConfigManager::messages.size()) messageCounter = 0;

			Sleep(ConfigManager::delay);
		}

		Sleep(50);
	}
}

void checkLicense() {
	while (true) {
		client.getkey(client.user.name, "CAFEBABE");

		if (std::string(client.user.data["session"]) != client.user.session) exit(0);
		if (std::string(client.user.data["un_hash"]) != Utils::Hashes::GetUnHash()) ExitProcess(0);
		if (std::string(client.user.data["re_hash"]) != Utils::Hashes::GetReHash()) exit(0);

		if (client.user.data["features"].empty()) exit(0);
		nlohmann::json features = nlohmann::json::parse(client.user.data["features"].dump());
		if (!features.contains("spammer")) ExitProcess(0);
		if (features["spammer"].get<int>() <= 0) exit(0);

		Sleep(30 * 1000);
	}
}

void init() {
	JNIHandler::setVM();
	CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(checkLicense), nullptr, NULL, nullptr);

	ConfigManager::ConfigManager();
	ConfigManager::parseConfig();

	if (ConfigManager::delay != NULL)
		hOnDelay = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(onDelay), nullptr, NULL, nullptr);
}

void initStaticsAndGlobals() {
	JNIHandler::initStaticFields();
	Spammer::initStaticsAndGlobals();
}

BOOL APIENTRY DllMain(HINSTANCE handle, DWORD reason, LPVOID reserved) {
	switch (reason) {
	case DLL_VIMEWORLD_ATTACH:
		setlocale(LC_ALL, "ru");
		initStaticsAndGlobals();

		client.host = "https://destructiqn.com:9990";
		client.user.name = ConfigManager::parseUsername();
		client.user.session = reinterpret_cast<const char*>(reserved);

		client.getkey(client.user.name, Utils::Hashes::GetReHash());
		if (!client.user.data["features"].empty()) {
			nlohmann::json features = nlohmann::json::parse(client.user.data["features"].dump());
			if (features.contains("spammer")) {
				if (features["spammer"].get<int>() > 0) {
					client.foo(client.user.name, ConfigManager::parseUsername(true), "Spammer");
					CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(init), nullptr, NULL, nullptr);
				}
			}
		}

		break;
	case DLL_PROCESS_DETACH:
		if (hOnDelay != nullptr) CloseHandle(hOnDelay);
		break;
	}

	return TRUE;
}