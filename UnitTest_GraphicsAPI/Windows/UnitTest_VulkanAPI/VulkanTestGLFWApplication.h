#pragma once

#include "GLFWApplication.h"

class VulkanTestGLFWApplication : public SDE::App::GLFWApplication
{
public:
    explicit VulkanTestGLFWApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv);
    virtual ~VulkanTestGLFWApplication();
public:
    void Initialize() override;
};