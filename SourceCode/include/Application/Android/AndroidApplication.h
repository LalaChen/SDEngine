/*==============  SD Engine License ==============
MIT License

Copyright(c) 2019 Kuan - Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/*! \file      AndroidApplication.h
 *  \brief     Introduce of class AndroidApplication.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/03/14
 *  \copyright MIT License.
 */

#pragma once

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "Application.h"

using SDE::Basic::Application;

________________SD_START_APP_NAMESPACE_______________

class SDENGINE_CLASS AndroidApplication : public Application
{
public:
    enum AppStateEnum {
        AppState_CREATE = 0,
        AppState_INITIALIZE,
        AppState_INITIALIZE_GRAPHICS,
        AppState_RUN,
        AppState_PAUSE,
        AppState_TERMINATE,
        AppState_MAX_DEFINE_VALUE
    };
public:
    /*! \fn explicit AndroidApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, int i_argc, char **i_argv);
     *  \param [in] i_win_title Window title.
     *  \param [in] i_asset_mgr Target asset manager.
     *  \param [in] i_adopt_library Which graphics library you use.
     *  \param [in] i_argc argument count.
     *  \param [in] i_argv arguments.
     *  \brief The constructor of AndroidApplication Class.
     */
    explicit AndroidApplication(const std::string &i_win_title, AAssetManager *i_asset_mgr, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv);

    /*! \fn virtual ~AndroidApplication();
     *  \brief The destructor of AndroidApplication Class.
     */
    ~AndroidApplication();
public:
    /*! \fn void Initialize() override;
     *  \brief Initialize this app. We will create WindowsLogManager, OpenGL4 API and Manager for GLFW.
     */
    void Initialize() override;

    /*! \fn void InitializeGraphicsSystem() override;
     *  \brief Initialize graphics system of this app.
     */
    void InitializeGraphicsSystem() override;

    /*! \fn void ReleaseGraphicsSystem() override;
     *  \brief release graphics system of this app.
     */
    void ReleaseGraphicsSystem() override;

    /*! \fn void TerminateApplication() override;
     *  \brief release all mamager for terminating app.
     */
    void TerminateApplication() override;

    void Resume() override;

    void Pause() override;
public:
    void InitializeNativeWindow(ANativeWindow *i_window);
    void RefreshNativeWindow(ANativeWindow *i_window, int i_width, int i_height);
    void RunMainLoop();
public:
    SD_DECLARE_ATTRIBUTE_VAR_GET(AppStateEnum, m_current_state, CurrentState);
protected:
    ANativeWindow *m_window;
    AAssetManager *m_asset_mgr;
    std::thread m_render_thread;
    std::mutex m_pause_mtx;
    std::condition_variable m_pause_cv;
    bool m_resize_signal;
};

_________________SD_END_APP_NAMESPACE________________