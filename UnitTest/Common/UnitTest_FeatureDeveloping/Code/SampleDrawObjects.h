#pragma once

#include "MotorComponent.h"
#include "Sample.h"

using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::Math;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(SampleDrawObjects);

class SampleDrawObjects : public Sample
{
public:
    explicit SampleDrawObjects(const ObjectName &i_sample_name);
    virtual ~SampleDrawObjects();
public:
    void Initialize() override;
    void Update() override;
    void Destroy() override;
    void Resize() override;
public:
    void InitializeScene() override;
    void UpdateScene() override;
    void DestroyScene() override;
protected:
    void LoadScene();
protected:
    EntityWeakReferenceObject m_scene_root_node;
    EntityWeakReferenceObject m_camera_node;
    EntityWeakReferenceObject m_light_node;
    EntityWeakReferenceObject m_axis_node;
    EntityWeakReferenceObject m_floor_node;
    std::list<EntityWeakReferenceObject> m_cube_objects;
protected:
    MeshStrongReferenceObject m_axis_mesh;
    MeshStrongReferenceObject m_floor_mesh;
    MeshStrongReferenceObject m_cube_mesh;
protected:
    MaterialStrongReferenceObject m_axis_material_sref;
    ShaderProgramStrongReferenceObject m_axis_shader_sref;
    MaterialStrongReferenceObject m_basic_material_sref;
protected:
    TextureStrongReferenceObject m_main_tex_sref;
protected:
    MotorComponentWeakReferenceObject m_camera_motor_wref;
protected:
    float m_cube_interval;
    float m_cube_side_length;
    uint32_t m_cube_row;
    uint32_t m_cube_col;
    uint32_t m_cube_depth;
};
