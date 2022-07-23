#include <jni.h>
#include "MMIPC.h"
#include "AndroidLog.h"
#include "Configs.h"

extern "C"
JNIEXPORT void
JNICALL
Java_com_zzy_mmipc_MMIPC_initMMAPJNI(JNIEnv *env, jobject thiz, jstring root_dir) {
    const char *dir = env->GetStringUTFChars(root_dir, nullptr);
    ALOGD("root dir %s", dir);
    string rootDir(dir);
    configs::get_instance().g_rootDir = rootDir;
    mmipc::get_instance().reloadMmap(dir);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_zzy_mmipc_MMIPC_setDataJNI(JNIEnv *env, jobject thiz, jstring key, jstring value) {
    const char *keyStr = env->GetStringUTFChars(key, nullptr);
    const char *valueStr = env->GetStringUTFChars(value, nullptr);
    mmipc::get_instance().setData(keyStr, valueStr);
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_zzy_mmipc_MMIPC_getDataJNI(JNIEnv *env, jobject thiz, jstring key, jstring defaultValue) {
    const char *keyStr = env->GetStringUTFChars(key, nullptr);
    const char *defaultValueStr = env->GetStringUTFChars(defaultValue, nullptr);
    std::string dataStr = mmipc::get_instance().getData(keyStr, defaultValueStr);
    if (dataStr.empty()) {
        ALOGD("getData empty by key%s", keyStr);
        dataStr = defaultValueStr;
    }
    return env->NewStringUTF(dataStr.c_str());
}