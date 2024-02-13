#include <jni.h>
#include "Texteria.h"
#include "../../../../../Handler.h"

string Texteria::tryGetServerId(string fallback) {
	jclass Texteria = JNIHandler::FindClass("net.xtrafrancyz.mods.texteria.Texteria");
	jmethodID mid = JNIHandler::env->GetStaticMethodID(Texteria, "tryGetServerId", "(Ljava/lang/String;)Ljava/lang/String;");
	jstring serverId = static_cast<jstring>(JNIHandler::env->CallStaticObjectMethod(Texteria, mid, JNIHandler::env->NewStringUTF(fallback.c_str())));
	
	jboolean isCopy = JNI_FALSE;
	const char* cServerId = JNIHandler::env->GetStringUTFChars(serverId, &isCopy);
	string server = string(cServerId);

	JNIHandler::env->ReleaseStringUTFChars(serverId, cServerId);
	JNIHandler::env->DeleteLocalRef(serverId);
	JNIHandler::env->DeleteLocalRef(Texteria);
	return server;
}