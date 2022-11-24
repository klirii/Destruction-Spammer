#include "CommonUtils.h"
#include "../../../../Handler.h"

void CommonUtils::sendMessage(jstring message) {
	jclass CommonUtils = JNIHandler::FindClass("net/xtrafrancyz/util/CommonUtils");
	jmethodID mid = JNIHandler::env->GetStaticMethodID(CommonUtils, "sendMessage", "(Ljava/lang/String;)V");
	JNIHandler::env->CallStaticVoidMethod(CommonUtils, mid, message);
}