#include "WorldGUIComponentBase.h"

#include "MathAlgoritm.h"
#include "MaterialUniforms.h"
#include "Application.h"
#include "ECSManager.h"
#include "BasicShapeCreator.h"
#include "GraphicsSystem.h"
#include "CameraComponentBase.h"

using namespace SDE::Math;
using namespace SDE::Basic;

_____________SD_START_GRAPHICS_NAMESPACE_____________

WorldGUIComponentBase::WorldGUIComponentBase(const ObjectName &i_object_name)
: Component(i_object_name)
, m_buffer_size{ 2, 2 }
, m_world_size{ 0.0f, 0.0f }
, m_is_ncp(false)
, m_area_orientation(AreaAlignOrientation_LEFT_BOTTOM)
{
}

WorldGUIComponentBase::~WorldGUIComponentBase()
{
}

void WorldGUIComponentBase::SetGUIAreaInWorldSpace(uint32_t i_width, uint32_t i_height, float i_world_w, float i_world_h)
{
    if (m_initialized == false) {
        m_is_ncp = false;
        m_world_size[0] = i_world_w;
        m_world_size[1] = i_world_h;
        m_buffer_size[0] = i_width;
        m_buffer_size[1] = i_height;
        float hw = i_world_w / 2.0f, hh = i_world_h / 2.0f;
        m_UI_vertices[0] = Vector3f(-hw, -hh, 0.0f, 1.0f);
        m_UI_vertices[1] = Vector3f(hw, -hh, 0.0f, 1.0f);
        m_UI_vertices[2] = Vector3f(hw, hh, 0.0f, 1.0f);
        m_UI_vertices[3] = Vector3f(-hw, hh, 0.0f, 1.0f);
    }
}

void WorldGUIComponentBase::SetGUIAreaInScreenSpace(AreaAlignOrientationEnum i_orientation, const Area2D &i_area)
{
    if (m_initialized == false) {
        m_is_ncp = true;
        m_screen_area = i_area;
        m_area_orientation = i_orientation;
        m_buffer_size[0] = m_screen_area.w;
        m_buffer_size[1] = m_screen_area.h;
    }
}

void WorldGUIComponentBase::InitializeImpl()
{
    m_transform = SD_GET_TYPE_COMP_WREF(m_entity, TransformComponent);
    UpdateArea();
}

void WorldGUIComponentBase::UpdateArea()
{  
    if (m_is_ncp == true) {
        CameraComponentBaseWeakReferenceObject camera;
        GraphicsSystemWeakReferenceObject gs = ECSManager::GetRef().GetSystem(typeid(GraphicsSystem)).DynamicCastTo<GraphicsSystem>();
        if (gs.IsNull() == false) {
            camera = SD_WREF(gs).GetScreenCamera();
        }

        if (camera.IsNull() == false) {
            m_world_area = SD_WREF(camera).ConvertScreenAreaToWorldArea(m_area_orientation, m_screen_area);
            m_world_size[0] = m_world_area.area.w;
            m_world_size[1] = m_world_area.area.h;
            //
            for (uint32_t orientation = 0; orientation < 4; ++orientation) {
                vec3 pos = m_world_area.GetPoint(orientation);
                m_UI_vertices[orientation] = Vector3f(pos.x, pos.y, pos.z, 1.0f);
            }
        }
        else {
            SDLOGW("Update area of GUI[%s] failure. Screen Camera is nullptr.", m_object_name.c_str());
        }
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________