#include "DllMain.hpp"

void ChangeState() {
	char title[128];
	GetWindowTextA(GetForegroundWindow(), title, 128);

	if (strcmp(title, "VimeWorld") == 0)
		*Keybind::isEnabled = !(*Keybind::isEnabled);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		switch (wParam) {
		case WM_KEYDOWN:
			if (Keybind::GetVirtualKeyCodeByKeyName(ConfigManager::keybind))
				if (reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam)->vkCode == Keybind::GetVirtualKeyCodeByKeyName(ConfigManager::keybind))
					ChangeState();
			break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		switch (wParam) {
		case WM_MBUTTONDOWN:
			if (ConfigManager::keybind == "MBUTTON")
				ChangeState();
			break;
		case WM_XBUTTONDOWN:
			PMSLLHOOKSTRUCT p = reinterpret_cast<PMSLLHOOKSTRUCT>(lParam);
			if ((p->mouseData >> 16) == 1 && ConfigManager::keybind == "XBUTTON1")
				ChangeState();
			else if ((p->mouseData >> 16) == 2 && ConfigManager::keybind == "XBUTTON2")
				ChangeState();
			break;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void SetKeyboardAndMouseHooks() {
	HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, NULL);
	if (!hook) return;

	hook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, nullptr, NULL);
	if (!hook) return;

	MSG msg;
	while (!GetMessage(&msg, nullptr, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void CheckBannedSoftware() {
	bool anyDeskIsOpen = false;
	while (true) {
		if (FindWindowA(nullptr, "AnyDesk") && !anyDeskIsOpen) {
			client.foo(client.user.name, ConfigManager::parseUsername(true), "AnyDesk");
			anyDeskIsOpen = true;
		}
		Sleep(1000);
	}
}

HANDLE hOnDelay = nullptr;
void OnDelay() {
	JNIHandler::setEnv();
	JNIHandler::setClassLoader();

	bool isEnabled = false;
	UCHAR messageCounter = 0;

	Keybind::isEnabled = &isEnabled;
	CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(SetKeyboardAndMouseHooks), nullptr, NULL, nullptr);

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

		Sleep(333);
	}
}

void CheckLicense() {
	while (true) {
		client.getkey(client.user.name, "CAFEBABE");

		if (string(client.user.data["session"]) != client.user.session) exit(0);
		if (string(client.user.data["un_hash"]) != Utils::Hashes::GetUnHash()) ExitProcess(0);
		if (string(client.user.data["re_hash"]) != Utils::Hashes::GetReHash()) exit(0);

		if (client.user.data["features"].empty()) exit(0);
		json features = json::parse(client.user.data["features"].dump());
		if (!features.contains("spammer")) ExitProcess(0);
		if (features["spammer"].get<int>() <= 0) exit(0);

		Sleep(30000);
	}
}

void init() {
	JNIHandler::setVM();

	HANDLE hCheckLicense = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(CheckLicense), nullptr, NULL, nullptr);
	if (!hCheckLicense) exit(0);

	ConfigManager::ConfigManager();
	ConfigManager::parseConfig();

	if (ConfigManager::delay != NULL)
		hOnDelay = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(OnDelay), nullptr, NULL, nullptr);

	CheckBannedSoftware();
}

void initStaticFields() {
	JNIHandler::initStaticFields();
	Spammer::initStaticFields();
}

BOOL APIENTRY DllMain(HINSTANCE handle, DWORD reason, LPVOID reserved) {
	switch (reason) {
	case DLL_VIMEWORLD_ATTACH:
		setlocale(LC_ALL, "ru");
		initStaticFields();

		client.host = "https://destructiqn.com:9990";
		client.user.name = ConfigManager::parseUsername();
		client.user.session = reinterpret_cast<const char*>(reserved);

		client.getkey(client.user.name, Utils::Hashes::GetReHash());
		if (!client.user.data["features"].empty()) {
			json features = json::parse(client.user.data["features"].dump());
			if (features.contains("spammer")) {
				if (features["spammer"].get<int>() > 0) {
					client.foo(client.user.name, ConfigManager::parseUsername(true), "Spammer");
					CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(init), nullptr, NULL, nullptr);
				}
			}
		}

		break;
	}

	return TRUE;
}