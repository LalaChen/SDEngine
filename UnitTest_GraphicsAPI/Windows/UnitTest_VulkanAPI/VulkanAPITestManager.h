#pragma once

#include "Sample1_DrawTriangle.h"
#include "VulkanManager.h"

class VulkanAPITestManager : public SDE::Graphics::VulkanManager
{
public:
    VulkanAPITestManager();
    virtual ~VulkanAPITestManager();
public:
    void InitializeGraphicsSystem(const EventArg &i_arg) override;
    void ReleaseGraphicsSystem() override;
public:
    void RenderToScreen() override;
    void RenderDebug();
protected:
    Sample1_DrawTriangle m_draw_triangle;
};