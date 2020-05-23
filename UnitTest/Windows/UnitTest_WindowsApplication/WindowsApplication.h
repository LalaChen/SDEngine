#pragma once

#include <windows.h>

#include "Application.h"

using SDE::Basic::KeyCodeEnum;
using SDE::Basic::KeyStatusEnum;

class WindowsApplication : public SDE::Basic::Application
{
public:
    explicit WindowsApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv);
    virtual ~WindowsApplication();
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

public:
    /*! \fn KeyStatusEnum GetKeyStateByCode(KeyCodeEnum i_code) override;
     *  \param [in] i_code Target key in keyboard.
     *  \brief Get key status by key code. Return not support if the key isn't supported.
     */
    KeyStatusEnum GetKeyStateByCode(KeyCodeEnum i_code) override;
public:
    void RunMainLoop();
protected:
    HWND m_hwnd;
    HINSTANCE m_h_ins;
};
