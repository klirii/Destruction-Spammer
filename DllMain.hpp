#define _CRT_SECURE_NO_WARNINGS
#define DLL_VIMEWORLD_ATTACH 0x888

#pragma warning(disable:4244)

#include <Windows.h>
#include <iostream>

#include "JNI/Handler.h"
#include "Config/ConfigManager.h"

#include "Utils/Strings/WString.h"
#include "Utils/Hashes.hpp"
#include "Utils/Keybind.hpp"

#include "Core/Spammer.h"
#include "Core/Settings/Keycodes.h"

#include "JNI/Classes/net/xtrafrancyz/util/CommonUtils.h"
#include "JNI/Classes/net/xtrafrancyz/mods/texteria/Texteria.h"

#include "RestAPI/Core/Client.hpp"
#include "RestAPI/Utils/Utils.hpp"

using namespace std;
using json = nlohmann::json;

static RestAPI::Client client;
