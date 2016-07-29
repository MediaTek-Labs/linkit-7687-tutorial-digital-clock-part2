#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <android/log.h>
#include "elian.h"

#define LOG_TAG    "elian-jni"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern "C" {
JNIEXPORT void JNICALL
        Java_com_mediatek_labs_digitalclock_MainActivity_elianInitSmartConnection(
        JNIEnv *env, jobject instance);
JNIEXPORT void JNICALL
        Java_com_mediatek_labs_digitalclock_MainActivity_elianStartSmartConnection(
        JNIEnv *env, jobject instance,
        jstring SSID,
        jstring password,
        jstring custom);
JNIEXPORT void JNICALL
        Java_com_mediatek_labs_digitalclock_MainActivity_elianStopSmartConnection(
        JNIEnv *env, jobject instance);
}

static void *g_context = NULL;

JNIEXPORT void JNICALL
Java_com_mediatek_labs_digitalclock_MainActivity_elianInitSmartConnection(JNIEnv *env, jobject instance) {

    unsigned char target[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    int protoVersion = 0;
    int libVersion = 0;

    elianGetVersion(&protoVersion, &libVersion);
    LOGI("elian protoVer=%d, libVer=%d", protoVersion, libVersion);

    if (g_context)
    {
        elianStop(g_context);
        elianDestroy(g_context);
        g_context = NULL;
    }

    g_context = elianNew(NULL, 0, target, ELIAN_SEND_V4);
    if (g_context == NULL) {
        LOGE("init fail!!");
    }
    else
    {
        LOGI("init success!");
        elianSetInterval(g_context, 20*1000);
    }
}

JNIEXPORT void JNICALL
Java_com_mediatek_labs_digitalclock_MainActivity_elianStartSmartConnection(JNIEnv *env, jobject instance,
                                                                           jstring SSID,
                                                                           jstring password,
                                                                           jstring custom) {

    const char *_ssid = NULL;
    const char *_password = NULL;
    const char *_custom = NULL;
    int result;

    _ssid = env->GetStringUTFChars(SSID, 0);
    _password = env->GetStringUTFChars(password, 0);
    _custom = env->GetStringUTFChars(custom, 0);

    LOGI("smart connection input: %s, %s, %s", _ssid, _password, _custom);

    elianPut(g_context, TYPE_ID_SSID, (char *)_ssid, strlen(_ssid));
    elianPut(g_context, TYPE_ID_PWD, (char *)_password, strlen(_password));
    elianPut(g_context, TYPE_ID_CUST, (char *)_custom, strlen(_custom));

    env->ReleaseStringUTFChars(SSID, _ssid);
    env->ReleaseStringUTFChars(password, _password);
    env->ReleaseStringUTFChars(custom, _custom);

    result = elianStart(g_context);
    LOGI("start smart connection: %d", result);
}

JNIEXPORT void JNICALL
Java_com_mediatek_labs_digitalclock_MainActivity_elianStopSmartConnection(JNIEnv *env, jobject instance) {

    if (!g_context)
        return;
    elianStop(g_context);
    LOGI("stop smart connection");
}