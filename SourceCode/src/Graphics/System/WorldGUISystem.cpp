/* ==============  SD Engine License ==============
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

#include "Application.h"
#include "Timer.h"
#include "ECSManager.h"
#include "WorldGUIComponent.h"
#include "WorldGUISystem.h"

using SDE::Basic::StringFormat;
using SDE::Basic::Application;
using SDE::Basic::ECSManager;
using SDE::Basic::Timer;

using SDE::Basic::MemberFunctionSlot;

using SDE::Basic::Entity;
using SDE::Basic::EntityStrongReferenceObject;
using SDE::Basic::EntityWeakReferenceObject;

_____________SD_START_GRAPHICS_NAMESPACE_____________

WorldGUISystem::WorldGUISystem(const ObjectName &i_object_name)
: System(i_object_name)
{
}

WorldGUISystem::~WorldGUISystem()
{
}

void WorldGUISystem::Initialize()
{
    m_GUI_cp = new CommandPool("GUICmdPool");
    SD_REF(m_GUI_cp).Initialize();
    m_GUI_cb = SD_REF(m_GUI_cp).AllocateCommandBuffer("GUICmdBuffer");

    m_world_GUI_eg = ECSManager::GetRef().AddEntityGroup(
        "WorldGUIGroup",
        {
            std::type_index(typeid(WorldGUIComponent))
        }
    );
}

void WorldGUISystem::Update()
{
    float delta_time = Timer::GetRef().GetProgramDeltaTime();
    if (delta_time > 0) {
        std::list<EntityWeakReferenceObject> world_gui_entities = SD_WREF(m_world_GUI_eg).GetEntities();
        for (EntityWeakReferenceObject &entity : world_gui_entities) {
            WorldGUIComponentWeakReferenceObject world_gui = SD_GET_COMP_WREF(entity, WorldGUIComponent);
            SD_WREF(world_gui).Update();
        }
    }
}

void WorldGUISystem::Resize()
{
}

void WorldGUISystem::Destroy()
{
    SD_SREF(m_GUI_cp).Clear();
    m_GUI_cp.Reset();
}

______________SD_END_GRAPHICS_NAMESPACE______________