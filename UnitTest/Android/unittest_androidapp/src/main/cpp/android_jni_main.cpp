#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <SDEngineMacro.h>
#include <SDEngineCommonFunction.h>
#include <SDEngine.h>

#include "AndroidApplication.h"
#include "android_log.h"

using SDE::App::AndroidApplication;

AndroidApplication *g_app = nullptr;

extern "C" JNIEXPORT
void JNICALL Java_com_sdengine_unittest_androidapp_InitializeNative(
        JNIEnv *env,
        jobject obj,
        jobject asset_mgr,
        jobject surface,
        jint format,
        jint w,
        jint h)
{
    //1. get ANativeWindow.
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    if (window != nullptr) {
        LOGE("We can't find the window from surface(%p).", surface);
        return;
    }

    //2. new Android Application.
    g_app = new AndroidApplication("test",
            SDE::Graphics::GraphicsLibrary_Vulkan,
            0, nullptr);

    g_app->Initialize();
}