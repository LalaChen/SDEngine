#pragma once

#include "SDEngine.h"
#include "FunctionSlot.h"
#include "GLFWApplication.h"

using SDE::Basic::EventObject;
using SDE::Basic::EventObjectStrongReferenceObject;
using SDE::Basic::EventObjectWeakReferenceObject;

class FeatureApplication : public SDE::App::GLFWApplication
{
public:
    explicit FeatureApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv);
    virtual ~FeatureApplication();
public:
    void Initialize() override;
};