#pragma once

#include <SDEngineMacro.h>
#include <SDEngineCommonType.h>
#include <SDEngineCommonFunction.h>
#include <SDEngine.h>

#include "MotorComponent.h"

using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::Math;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(SampleDrawObjects);

class SampleDrawObjects : public Scene
{
public:
    explicit SampleDrawObjects(const ObjectName &i_scene_name);
    virtual ~SampleDrawObjects();
public:
    bool LoadImpl() override;
    void UnloadImpl() override;
protected:
    EntityWeakReferenceObject m_scene_root_node;
    EntityWeakReferenceObject m_camera_node;
    EntityWeakReferenceObject m_WGUI_node;
    EntityWeakReferenceObject m_light_node;
    EntityWeakReferenceObject m_axis_node;
    EntityWeakReferenceObject m_floor_node;
    EntityWeakReferenceObject m_screen_ray_node;
    std::list<EntityWeakReferenceObject> m_cube_objects;
protected:
    MeshStrongReferenceObject m_axis_mesh;
    MeshStrongReferenceObject m_floor_mesh;
    MeshStrongReferenceObject m_cube_mesh;
protected:
    MaterialStrongReferenceObject m_axis_material;
    ShaderProgramStrongReferenceObject m_axis_shader;
    MaterialStrongReferenceObject m_basic_material;
protected:
    TextureStrongReferenceObject m_main_tex;
protected:
    MotorComponentWeakReferenceObject m_camera_motor;
protected:
    float m_cube_interval;
    float m_cube_side_length;
    uint32_t m_cube_row;
    uint32_t m_cube_col;
    uint32_t m_cube_depth;
};
