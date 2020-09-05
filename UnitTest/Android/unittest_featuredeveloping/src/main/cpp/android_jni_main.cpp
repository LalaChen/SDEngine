#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <SDEngineMacro.h>
#include <SDEngineCommonFunction.h>
#include <SDEngine.h>

#include "FeatureApplication.h"
#include "android_log.h"
#include <jni.h>
#include <jni.h>
#include <jni.h>
#include <jni.h>

FeatureApplication *g_app = nullptr;

void TerminateApplication()
{
	if (g_app != nullptr) {
		LOGI("Termination application.");
        g_app->TerminateApplication();
		delete g_app;
		g_app = nullptr;
    }
}

extern "C" JNIEXPORT
void JNICALL Java_com_sdengine_unittest_1featuredeveloping_MainActivity_InitializeApplication(
        JNIEnv *env,
        jclass clazz,
        jobject j_asset_mgr)
{
	LOGI("Create App instance.");
    // TODO: implement InitializeApplication()
    AAssetManager *asset_mgr = AAssetManager_fromJava(env, j_asset_mgr);
    //1. new Android Application.
	if (g_app != nullptr) {
		TerminateApplication();
	}

    g_app = new FeatureApplication("test",
                                   asset_mgr,
                                   SDE::Graphics::GraphicsLibrary_Vulkan,
                                   0, nullptr);

    g_app->Initialize();
}

extern "C" JNIEXPORT
void JNICALL Java_com_sdengine_unittest_1featuredeveloping_MainActivity_ChangeSurface(
        JNIEnv *env,
        jclass clazz,
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
		AndroidApplication::AppStateEnum current_state = g_app->GetCurrentState();
        if (current_state == AndroidApplication::AppState_CREATE || current_state == AndroidApplication::AppState_INITIALIZE) {
			LOGI("NativeWindow(%p) State is (%d) create or initialize.", window, current_state);
            g_app->InitializeNativeWindow(window);
            g_app->RunMainLoop();
        }
        else {
            if (current_state == AndroidApplication::AppState_PAUSE) {
				LOGI("NativeWindow(%p) State is at Pause. Refresh native window.", window);
                g_app->RefreshNativeWindow(window, width, height);
            }
            else {
                LOGI("Wrong AppState(%d)", g_app->GetCurrentState());
            }
        }
    }
}

extern "C" JNIEXPORT
void JNICALL Java_com_sdengine_unittest_1featuredeveloping_MainActivity_Pause(
        JNIEnv *env,
        jclass clazz)
{
    if (g_app != nullptr) {
		LOGI("Pause application.");
        g_app->Pause();
    }
}

extern "C" JNIEXPORT
void JNICALL Java_com_sdengine_unittest_1featuredeveloping_MainActivity_TerminateApplication(
        JNIEnv *env,
        jclass clazz)
{
	TerminateApplication();
}