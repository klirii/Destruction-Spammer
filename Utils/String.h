#pragma once
#include <iostream>
using namespace std;

class String {
public:
	String(const char* cStr);
	string toLower();
private:
	const char* str;
};