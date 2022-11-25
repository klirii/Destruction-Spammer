#include "String.h"
#include <algorithm>

String::String(const char* cStr) {
	this->str = cStr;
}

string String::toLower() {
	string str = this->str;
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}