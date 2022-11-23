#pragma once
#include <jni.h>
#include <Windows.h>

class JNIHandler {
public:
	static JNIEnv* env;
	static jobject ClassLoader;

	static void setVM();
	static void setEnv();
	static void setClassLoader();

	static jclass FindClass(const char* name);
private:
	static JavaVM* vm;
	static HMODULE jvm;

	typedef jclass(JNICALL* pJVM_FindClassFromCaller)(JNIEnv* env, const char* name, jboolean init, jobject loader, jclass caller);
	static  pJVM_FindClassFromCaller JVM_FindClassFromCaller;
};