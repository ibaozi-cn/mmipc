#include <jni.h>
#include "MMIPC.h"
#include "AndroidLog.h"

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_zzy_mmipc_NativeLib_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_zzy_mmipc_NativeLib_initMMAP(JNIEnv *env, jobject thiz, jstring root_dir) {
    const char *dir = env->GetStringUTFChars(root_dir, nullptr);
    ALOGD("open file %s", dir);
    MMIPC::open(dir);
    MMIPC::initMMAP();
    std::string hello = "Hello from MMIPC";
    return env->NewStringUTF(hello.c_str());
}