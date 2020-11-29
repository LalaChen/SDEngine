#pragma once

#include "SDEngine.h"
#include "FunctionSlot.h"
#include "GLFWApplication.h"
#include "Sample.h"

using SDE::Basic::EventObject;
using SDE::Basic::EventObjectStrongReferenceObject;
using SDE::Basic::EventObjectWeakReferenceObject;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(ApplicationManipulater);

class ApplicationManipulater : public EventObject
{
public:
    ApplicationManipulater(const ObjectName &i_object_name);
    ~ApplicationManipulater();
public:
    void Initalize();
    void Release();
public:
    bool OnReceiveKeyStateChanged(const EventArg &i_arg);
};

class FeatureApplication : public SDE::App::GLFWApplication
{
public:
    explicit FeatureApplication(const std::string &i_win_title, const Resolution &i_win_res, FullWindowOption i_full_window, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv);
    virtual ~FeatureApplication();
public:
    void Initialize() override;
protected:
    ApplicationManipulaterStrongReferenceObject m_app_manipulater;
};