/*==============  SD Engine License ==============
MIT License

Copyright (c) 2019 Kuan-Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/*! \file      OpenXRApplication.h
 *  \brief     Introduce of class OpenXRApplication
 *  \author    Kuan-Chih, Chen
 *  \date      2022/09/02
 *  \copyright MIT License.
 */

#pragma once

#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

#include "SDEngineMacro.h"

#include "OpenXRConfig.h"
#include "Application.h"

using SDE::Basic::KeyStatusEnum;
using SDE::Basic::KeyCodeEnum;
using SDE::Basic::Application;

______________SD_START_OPENXR_NAMESPACE______________

class SDENGINE_CLASS OpenXRApplication : public Application
{
public:
    OpenXRApplication(
        const std::string &i_win_title,
        const Resolution &i_win_res,
        FullWindowOption i_full_window,
        GraphicsLibraryEnum i_adopt_library,
        int i_argc, char** i_argv);

    ~OpenXRApplication();
public:
    /*! \fn virtual void Initialize() override;
     *  \brief Initialize this game app. Please call this function once at executable
     *         starting. Please implement this function by derived class.
     */
    void Initialize() override;

public:
    /*! \fn virtual void TerminateApplication() override;
     *  \brief release all mamager for terminating app.
     */
    void TerminateApplication() override;
public:
    /*! \fn virtual KeyStatusEnum GetKeyStateByCode(KeyCodeEnum i_code) override;
     *  \param [in] i_code Target key in keyboard.
     *  \brief Get key status by key code. Return not support if the key isn't supported.
     */
    KeyStatusEnum GetKeyStateByCode(KeyCodeEnum i_code) override;
};

_______________SD_END_OPENXR_NAMESPACE_______________