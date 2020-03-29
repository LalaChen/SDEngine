#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <SDEngineMacro.h>
#include <SDEngineCommonFunction.h>
#include <SDEngine.h>

#include "TestAndroidApplication.h"
#include "android_log.h"

TestAndroidApplication *g_app = nullptr;

extern "C" JNIEXPORT
void JNICALL Java_com_sdengine_unittest_1androidapp_MainActivity_InitializeApplication(
        JNIEnv *env,
        jobject obj,
        jobject j_asset_mgr)
{
    AAssetManager *asset_mgr = AAssetManager_fromJava(env, j_asset_mgr);
    //1. new Android Application.
    g_app = new TestAndroidApplication("test",
            asset_mgr,
            SDE::Graphics::GraphicsLibrary_Vulkan,
            0, nullptr);

    g_app->Initialize();
}

extern "C" JNIEXPORT
void JNICALL Java_com_sdengine_unittest_1androidapp_MainActivity_ChangeSurface(
        JNIEnv *env,
        jobject obj,
        jobject surface,
        jint format,
        jint width,
        jint height)
{
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    if (window == nullptr) {
        LOGE("We can't find the window from surface(%p).", surface);
        return;
    }
    LOGI("NativeWindow(%p) from surface(%p)with(%d,%d,(%d))", window, surface, width, height, format);
    if (g_app != nullptr) {
        if (g_app->GetCurrentState() == AndroidApplication::AppState_CREATE || g_app->GetCurrentState() == AndroidApplication::AppState_INITIALIZE) {
            g_app->InitializeNativeWindow(window);
            g_app->RunMainLoop();
        }
        else {
            if (g_app->GetCurrentState() == AndroidApplication::AppState_PAUSE) {
                g_app->RefreshNativeWindow(window, width, height);
            }
            else {
                LOGI("Wrong AppState(%d)", g_app->GetCurrentState());
            }
        }
    }
}

extern "C" JNIEXPORT
void JNICALL Java_com_sdengine_unittest_1androidapp_MainActivity_Pause(
        JNIEnv *env,
        jobject obj)
{
    if (g_app != nullptr) {
        g_app->Pause();
    }
}