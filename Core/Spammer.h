#pragma once
#include <iostream>
#include <Windows.h>
using namespace std;

class Spammer {
public:
	static void initStaticFields();
	static wstring getFormattedMessage(const char* message);
private:
	static byte ReplaceProbability;

	static byte prevMessagesCounter;
	static wstring prevMessages[5];
};