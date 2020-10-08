#pragma once

#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "SDEngine.h"

using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::Math;

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Camera);

class Camera : public Object
{
public:
    explicit Camera(const ObjectName &i_object_name);
    virtual ~Camera();
public:
    void SetPerspective(float i_angle, float i_aspect, float i_near, float i_far);
    void SetRenderPass(const RenderPassWeakReferenceObject &i_rp_wref);
public:
    void Initialize();
    void Resize();
public:
    //void Render(const CommandBufferWeakReferenceObject &i_cb_wref, const std::list<MeshRenderWeakReferenceObject> &i_render_wref);
public: //Temporary transform function.
    void AddTranslation(const Vector3f &i_offset);
    void AddRotationX(float i_angle);
    void AddRotationY(float i_angle);
    void AddRotationZ(float i_angle);
    void SetTransform(const Transform &i_trans);
public:
    RenderFlowStrongReferenceObject m_rf_sref;
public:
    TextureStrongReferenceObject m_color_buffer;
    TextureStrongReferenceObject m_depth_buffer;
public:
    Transform m_trans;
public:
    RenderPassWeakReferenceObject m_custom_rp_wref;
public:
    ClearValue m_clear_color;
    ClearValue m_clear_d_and_s;
    Matrix4X4f m_proj_mat;
    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;
};