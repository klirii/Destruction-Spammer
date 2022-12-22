#include "Spammer.h"
#include "Settings/Dictionary.h"

#include "../Utils/Time.h"
#include "../Utils/Strings/WString.h"

byte Spammer::ReplaceProbability = 75;

byte Spammer::prevMessagesCounter = 0;
wstring Spammer::prevMessages[5];

wstring Spammer::getFormattedMessage(const char* message) {
	wstring formattedMessage = L"";

	for (byte i = 0; message[i]; i++) {
		srand(static_cast<unsigned int>(Time::now.UnixNano()));
		if (Dictionary.count(message[i])) {
			if (message[i] == '.') {
				formattedMessage += Dictionary[message[i]][0];
				continue;
			}

			if ((rand() % 100 + 1) > Spammer::ReplaceProbability) {
				formattedMessage += WString::GetSymbolFromCString(message, i);
				continue;
			}

			byte valCount = static_cast<byte>(Dictionary[message[i]].size());
			int selValIndex = rand() % valCount;

			if (Spammer::prevMessagesCounter > 0 && valCount > 1) {
				for (byte j = 0; j < Spammer::prevMessagesCounter; j++) {
					byte prevMessageLen = static_cast<byte>(wcslen(Spammer::prevMessages[j].c_str()));
					for (byte k = 0; k < prevMessageLen; k++) {
						if (Spammer::prevMessages[j][k] == Dictionary[message[i]][selValIndex][0]) {
							int selValIndexBuf = selValIndex;
							while (selValIndexBuf == selValIndex) {
								srand(static_cast<unsigned int>(Time::now.UnixNano()));
								selValIndex = rand() % valCount;
							}
						}
					}
				}
			}

			formattedMessage += Dictionary[message[i]][selValIndex];
		}
		else
			formattedMessage += WString::GetSymbolFromCString(message, i);
		Sleep(1);
	}

	Spammer::prevMessages[Spammer::prevMessagesCounter] = formattedMessage;
	Spammer::prevMessagesCounter++;

	if (Spammer::prevMessagesCounter == 5) Spammer::prevMessagesCounter = 0;
	return formattedMessage;
}