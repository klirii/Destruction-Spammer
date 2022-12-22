#pragma once
#include <iostream>
using namespace std;

class String {
public:
	String(const char* cStr);
	string toLower();
	string toUpper();
private:
	const char* str;
};