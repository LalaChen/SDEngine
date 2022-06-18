#include "ScreenRayComponent.h"

ScreenRayComponent::ScreenRayComponent(const ObjectName &i_name)
: Component(i_name)
{
}

ScreenRayComponent::~ScreenRayComponent()
{
}

void ScreenRayComponent::InitializeImpl()
{
    m_mesh_render = SD_WREF(m_entity).GetComponentByType<MeshRenderComponent>().StaticCastTo<MeshRenderComponent>();
    if (m_mesh_render.IsNull() == true) {
        m_mesh_render = ECSManager::GetRef().AddComponentForEntity<MeshRenderComponent>(
            m_entity, SDE::Basic::StringFormat("%s_MeshRender", m_object_name.c_str())).DynamicCastTo<MeshRenderComponent>();
        SD_WREF(m_mesh_render).Initialize();
    }
    //2.1 create material.
    m_material = new Material("GUIMaterial");
    SD_SREF(m_material).BindShaderProgram(GraphicsManager::GetRef().GetShaderProgram("AxesShader"));
    //Set data done. Link with shader program.(Write descirptor)
    SD_SREF(m_material).LinkWithShaderProgram();
    MaterialUniforms mat_ub; //use default color.
    SD_SREF(m_material).SetDataToUniformBuffer(sUniformBuffer_Material, &mat_ub, sizeof(MaterialUniforms));
    SD_SREF(m_material).Update();

    m_mesh = BasicShapeCreator::GetRef().CreateCircle(Vector3f::PositiveX, Vector3f::PositiveY.negative(), 0.1f);
    SD_SREF(m_mesh_render).AppendMesh(m_mesh, m_material);

    m_transform = SD_GET_COMP_WREF(m_entity, TransformComponent);
}

void ScreenRayComponent::UpdateImpl()
{
    GraphicsSystemWeakReferenceObject gs = ECSManager::GetRef().GetSystem(typeid(GraphicsSystem)).DynamicCastTo<GraphicsSystem>();
    if (gs.IsNull() == false) {
        CameraComponentWeakReferenceObject camera = SD_WREF(gs).GetScreenCamera().DynamicCastTo<CameraComponent>();
        if (camera.IsNull() == false) {
            TouchButton tb = Application::GetRef().GetTouchButton(TouchButton_RIGHT);
            Ray ray = SD_WREF(camera).CalculateRay(tb);
            Transform ray_xform = ray.CalculateTransform();
            ray_xform.AddTranslation(ray_xform.GetForward().scale(10.0f));
            SD_WREF(m_transform).SetWorldTransform(ray_xform);
        }
        else {
            SDLOGW("Camera is null. We can't update ray.");
        }
    }
}