#include <jni.h>
#include "MMIPC.h"
#include "AndroidLog.h"

static MMIPC *mmipc = nullptr;

extern "C"
JNIEXPORT jstring
JNICALL
Java_com_zzy_mmipc_MMIPC_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void
JNICALL
Java_com_zzy_mmipc_MMIPC_initMMAP(JNIEnv *env, jobject thiz, jstring root_dir) {
    const char *dir = env->GetStringUTFChars(root_dir, nullptr);
    ALOGD("open file %s", dir);
    mmipc = new MMIPC();
    mmipc->reloadMmap(dir);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_zzy_mmipc_MMIPC_setData(JNIEnv *env, jobject thiz, jstring key, jstring value) {
    const char *keyStr = env->GetStringUTFChars(key, nullptr);
    const char *valueStr = env->GetStringUTFChars(value, nullptr);
    if (mmipc) {
        mmipc->setData(keyStr, valueStr);
    }
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_zzy_mmipc_MMIPC_getData(JNIEnv *env, jobject thiz, jstring key, jstring defaultValue) {
    const char *keyStr = env->GetStringUTFChars(key, nullptr);
    const char *defaultValueStr = env->GetStringUTFChars(defaultValue, nullptr);
    if (mmipc) {
        std::string dataStr = mmipc->getData(keyStr, defaultValueStr);
        if (dataStr.empty()) {
            ALOGD("getData empty by key%s", keyStr);
            dataStr = defaultValueStr;
        }
        ALOGD("getData result by key=%s", dataStr.c_str());
        return env->NewStringUTF(dataStr.c_str());
    }
}