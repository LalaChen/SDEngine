#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <wvr/wvr.h>

#include <SDEngineMacro.h>
#include <SDEngineCommonFunction.h>
#include <SDEngine.h>

#include "WaveVRApplication.h"

#include "android_log.h"

WaveVRApplication *g_app = nullptr;

int main(int argc, char **argv)
{
    if (g_app != nullptr) {
        g_app->Initialize();
        g_app->RunMainLoop();
        delete g_app;
        g_app = nullptr;
    }
}

extern "C" JNIEXPORT
void JNICALL Java_com_sdengine_wavevrapp_MainActivity_InitializeApplication(
        JNIEnv *env,
        jclass clazz,
        jobject j_asset_mgr) {
    LOGI("Create App instance.");
    AAssetManager *asset_mgr = AAssetManager_fromJava(env, j_asset_mgr);

    //1. Check application have been created or not.
    //   Orientation changing will trigger onCreate, so we need to do this.
    //   That is, we will terminate app if orientation changed.
    if (g_app == nullptr) {
        g_app = new WaveVRApplication("WaveVR",
                                      asset_mgr,
                                      SDE::Graphics::GraphicsLibrary_Vulkan,
                                      0, nullptr);
        WVR_RegisterMain(main);
    }
    else {
        LOGI("Application already be created.");
        throw std::runtime_error("Application already be created.");
    }
}

extern "C" JNIEXPORT
void JNICALL Java_com_sdengine_wavevrapp_MainActivity_Pause(
        JNIEnv *env,
        jclass clazz)
{
    if (g_app != nullptr) {
        LOGI("Pause application.");
        g_app->Pause();
    }
}

extern "C" JNIEXPORT
void JNICALL Java_com_sdengine_wavevrapp_MainActivity_Resume(
        JNIEnv *env,
        jclass clazz)
{
    if (g_app != nullptr) {
        LOGI("Resume application.");
        g_app->Resume();
    }
}

extern "C" JNIEXPORT
void JNICALL Java_com_sdengine_wavevrapp_MainActivity_TerminateApplication(
        JNIEnv *env,
        jclass clazz)
{
    if (g_app != nullptr) {
        LOGI("Notify terminating application.");
        g_app->NotifyTerminating();
    }
}