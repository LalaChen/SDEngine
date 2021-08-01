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

* /

/*! \file      WaveVRApplication.h
 *  \brief     Introduce of class WaveVRApplication.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/05/29
 *  \copyright MIT License.
 */

#pragma once

#include "WaveCommonFunction.h"
#include "WaveVREventArg.h"

using SDE::Basic::KeyCodeEnum;
using SDE::Basic::KeyStatusEnum;
using SDE::Basic::Application;
using SDE::Basic::EventObject;
using SDE::Basic::EventObjectStrongReferenceObject;
using SDE::Basic::EventObjectWeakReferenceObject;

class WaveVRApplication : public Application
{
public:
    explicit WaveVRApplication(const std::string &i_win_title, AAssetManager *i_asset_mgr, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv);
    virtual ~WaveVRApplication();
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
    void HandleInput();
public:
    void RunMainLoop();
public:
    /*! \fn KeyStatusEnum GetKeyStateByCode(KeyCodeEnum i_code) override;
     *  \param [in] i_code Target key in keyboard.
     *  \brief Get key status by key code. Return not support if the key isn't supported.
     */
    KeyStatusEnum GetKeyStateByCode(KeyCodeEnum i_code) override;

    void NotifyTerminating();

protected:
    AAssetManager *m_asset_mgr;
    std::mutex m_main_loop_mtx;
    bool m_is_app_terminate;
};

inline void WaveVRApplication::NotifyTerminating()
{
    std::lock_guard<std::mutex> lock(m_main_loop_mtx);
    m_is_app_terminate = true;
}