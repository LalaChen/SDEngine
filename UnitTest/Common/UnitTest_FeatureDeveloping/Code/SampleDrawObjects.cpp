#include "SDEngine.h"
#include "HUDComponent.h"
#include "SampleDrawObjects.h"

using namespace SDE::Math;
using namespace SDE::Basic;
using namespace SDE::Graphics;
using namespace SDE::GUI;

SampleDrawObjects::SampleDrawObjects(const ObjectName &i_scene_name)
: Scene(i_scene_name, "SampleDrawObjects")
, m_cube_interval(0.05f)
, m_cube_side_length(0.25f)
#if defined(_WIN32) || defined(_WIN64)
, m_cube_row(15)
, m_cube_col(15)
, m_cube_depth(15)
#else
, m_cube_row(10)
, m_cube_col(10)
, m_cube_depth(10)
#endif
{
}

SampleDrawObjects::~SampleDrawObjects()
{
}

bool SampleDrawObjects::Load()
{
    bool result = Scene::Load();
    if (result == false) {
        SDLOGW("Load scene failure.");
        return result;
    }
    //1. allocate shader programs.
    {
        //GraphicsManager::GetRef().RegisterShaderProgram("AxesShader", "ShaderProgram/AxesShading/AxesShading.json");
        //GraphicsManager::GetRef().RegisterShaderProgram("BasicShader", "ShaderProgram/BasicShading/BasicShading.json");
        //GraphicsManager::GetRef().RegisterShaderProgram("NoLightShader", "ShaderProgram/NoLightShading/NoLightShading.json");
        //GraphicsManager::GetRef().RegisterShaderProgram("GUIShader", "ShaderProgram/GUIShading/GUIShading.json");
    }
    {
        m_main_tex = new Texture("MainTexture");
        m_main_tex.GetRef().SetSamplerFilterType(SamplerFilterType_LINEAR, SamplerFilterType_LINEAR);
        m_main_tex.GetRef().InitializeFromImageResource("Texture/Lenna.png");
    }
    {
        m_axis_material = new Material("AxesMaterial");
        m_axis_material.GetRef().BindShaderProgram(GraphicsManager::GetRef().GetShaderProgram("AxesShader"));
        //Set data done. Link with shader program.(Write descirptor)
        m_axis_material.GetRef().LinkWithShaderProgram();
        MaterialUniforms mat_ub; //use default color.
        m_axis_material.GetRef().SetDataToUniformBuffer(sUniformBuffer_Material, &mat_ub, sizeof(MaterialUniforms));
        m_axis_material.GetRef().Update();
    }
    {
        m_basic_material = new Material("BasicMaterial");
        m_basic_material.GetRef().BindShaderProgram(GraphicsManager::GetRef().GetShaderProgram("BasicShader"));
        //Set data done. Link with shader program.(Write descirptor)
        m_basic_material.GetRef().LinkWithShaderProgram();
        MaterialUniforms mat_ub; //use default color.
        m_basic_material.GetRef().SetDataToUniformBuffer(sUniformBuffer_Material, &mat_ub, sizeof(MaterialUniforms));
        m_basic_material.GetRef().SetTexture(sUniformImages_Material_Textures, m_main_tex, 0);
        m_basic_material.GetRef().Update();
    }

    //4. allocate scene root.
    m_scene_root_node = (*m_entities.begin());
    //5. Add camera.
    m_camera_node = ECSManager::GetRef().CreateEntity("Camera");
    m_entities.push_back(m_camera_node);
    ECSManager::GetRef().AddComponentForEntity<TransformComponent>(m_camera_node, "CameraTransform");
    SD_TYPE_COMP_WREF(m_scene_root_node, TransformComponent).AddChild(SD_GET_TYPE_COMP_WREF(m_camera_node, TransformComponent));

    SD_TYPE_COMP_WREF(m_camera_node, TransformComponent).SetWorldTransform(
        Transform::LookAt(
            Vector3f(1.0f, 1.5f, 7.0f, 1.0f),
            Vector3f(0.0f, 1.5f, 0.0f, 1.0f),
            Vector3f::PositiveY,
            true));
#if defined(SD_VR_MODE)
    SDLOG("Initialize VRCamera");
    ECSManager::GetRef().AddComponentForEntity<VRCameraComponent>(m_camera_node, "Camera");
    SD_TYPE_COMP_WREF(m_camera_node, VRCameraComponent).SetClearValues(
        { 0.1f, 0.1f, 0.1f, 1.0f },
        { 1.0f, 1 });
    Matrix4X4f projs[VREye_Both];
    SD_TYPE_COMP_WREF(m_camera_node, VRCameraComponent).Initialize();
#else
    SDLOG("Initialize Camera");
    ECSManager::GetRef().AddComponentForEntity<CameraComponent>(m_camera_node, "Camera");
    SD_TYPE_COMP_WREF(m_camera_node, CameraComponent).SetClearValues(
        { 0.35f, 0.35f, 0.75f, 1.0f },
        { 1.0f, 1 });
    SD_TYPE_COMP_WREF(m_camera_node, CameraComponent).SetPerspective(120, 0.01f, 1000.0f);
    SD_TYPE_COMP_WREF(m_camera_node, CameraComponent).Initialize();

    ECSManager::GetRef().AddComponentForEntity<MotorComponent>(m_camera_node, "CameraMotor");
    m_camera_motor = SD_GET_TYPE_COMP_WREF(m_camera_node, MotorComponent);
    SD_WREF(m_camera_motor).Initialize();
#endif
    SDLOG("Initialize WGUI");
    //6. WorldGUI.
    m_WGUI_node = ECSManager::GetRef().CreateEntity("WGUI");
    m_entities.push_back(m_WGUI_node);
    ECSManager::GetRef().AddComponentForEntity<TransformComponent>(m_WGUI_node, "WGUITransform");
    SD_COMP_WREF(m_WGUI_node, TransformComponent).SetLocalPosition(Vector3f(0.165f, 0.087f, -0.5f, 1.0f));
    SD_TYPE_COMP_WREF(m_camera_node, TransformComponent).AddChild(SD_GET_TYPE_COMP_WREF(m_WGUI_node, TransformComponent));

    ECSManager::GetRef().AddComponentForEntity<WorldGUIComponent>(m_WGUI_node, "WGUI");
    SD_TYPE_COMP_WREF(m_WGUI_node, WorldGUIComponent).SetBufferSize(360, 360);
    SD_TYPE_COMP_WREF(m_WGUI_node, WorldGUIComponent).SetWorldSize(0.15f, 0.15f);
    SD_TYPE_COMP_WREF(m_WGUI_node, WorldGUIComponent).Initialize();
    SD_TYPE_COMP_WREF(m_WGUI_node, WorldGUIComponent).LoadGUI(
        [](const IMGUIBatchWeakReferenceObject &i_batch) -> bool {
            IMGUIWindowStrongReferenceObject window = new IMGUIWindow("CameraWGUI", "HUD");
            IMGUIRect rect;
            rect.m_pos.x = 0;
            rect.m_pos.y = 0;
            rect.m_size.x = 360;
            rect.m_size.y = 360;
            SD_SREF(window).SetUIVertices(rect);
            //
            IMGUITextLabelStrongReferenceObject text_label = new IMGUITextLabel("FPSLabel", "");
            SD_SREF(window).Append(text_label.StaticCastTo<IMGUINode>());
            SD_WREF(i_batch).AddWindow(window);
            //
            return true;
        }
    );

    ECSManager::GetRef().AddComponentForEntity<HUDComponent>(m_WGUI_node, "HUDComponent");
    SD_TYPE_COMP_WREF(m_WGUI_node, HUDComponent).Initialize();

    //7. Add light.
    m_light_node = ECSManager::GetRef().CreateEntity("Light");
    m_entities.push_back(m_light_node);
    ECSManager::GetRef().AddComponentForEntity<TransformComponent>(m_light_node, "LightTransform");
    ECSManager::GetRef().AddComponentForEntity<LightComponent>(m_light_node, "Light");
    SD_TYPE_COMP_WREF(m_scene_root_node, TransformComponent).AddChild(SD_GET_TYPE_COMP_WREF(m_light_node, TransformComponent));
    SD_TYPE_COMP_WREF(m_light_node, LightComponent).SetLightParameter(LightUniforms());
    SD_TYPE_COMP_WREF(m_light_node, LightComponent).Initialize();

    SD_COMP_WREF(m_light_node, TransformComponent).SetWorldTransform(
        Transform::LookAt(Vector3f(1.0f, 3.0f, 1.0f, 1.0f), Vector3f::Origin, Vector3f::PositiveY)
    );

    //8. add axis.
    m_axis_mesh = BasicShapeCreator::GetRef().CreateAxis(0.2f, 20.0f);

    m_axis_node = ECSManager::GetRef().CreateEntity("AxisNode");
    m_entities.push_back(m_axis_node);

    ECSManager::GetRef().AddComponentForEntity<TransformComponent>(m_axis_node, "AxisTransform");
    ECSManager::GetRef().AddComponentForEntity<MeshRenderComponent>(m_axis_node, "AxisMeshRender");
    SD_TYPE_COMP_WREF(m_scene_root_node, TransformComponent).AddChild(SD_GET_TYPE_COMP_WREF(m_axis_node, TransformComponent));

    SD_TYPE_COMP_WREF(m_axis_node, TransformComponent).SetWorldPosition(Vector3f(0.0f, 0.0001f, 0.0f));
    SD_TYPE_COMP_WREF(m_axis_node, MeshRenderComponent).Initialize();
    SD_TYPE_COMP_WREF(m_axis_node, MeshRenderComponent).AppendMesh(m_axis_mesh, m_axis_material);

    //9. add floor.
    m_floor_mesh = BasicShapeCreator::GetRef().CreatePlane(
        Vector3f::Zero, Vector3f::PositiveZ, Vector3f::PositiveX,
        100.0f, 100.0f, 100.0f, 100.0f);

    m_floor_node = ECSManager::GetRef().CreateEntity("FloorNode");
    m_entities.push_back(m_axis_node);

    ECSManager::GetRef().AddComponentForEntity<TransformComponent>(m_floor_node, "FloorTransform");
    ECSManager::GetRef().AddComponentForEntity<MeshRenderComponent>(m_floor_node, "FloorMeshRender");
    SD_TYPE_COMP_WREF(m_scene_root_node, TransformComponent).AddChild(SD_GET_TYPE_COMP_WREF(m_floor_node, TransformComponent));

    SD_TYPE_COMP_WREF(m_floor_node, MeshRenderComponent).Initialize();
    SD_TYPE_COMP_WREF(m_floor_node, MeshRenderComponent).AppendMesh(m_floor_mesh, m_basic_material);

    //10. add cubes.
    m_cube_mesh = BasicShapeCreator::GetRef().CreateCube(Vector3f::Zero, Vector3f(0.25f, 0.25f, 0.25f));
    
    Vector3f start_pos = Vector3f(m_cube_side_length * 2.0f, m_cube_side_length * 2.0f, m_cube_side_length * 2.0f, 1.0f).scale(
        -1.0f * static_cast<float>(m_cube_row) / 2.0f,
        0.25f,
        -1.0f * static_cast<float>(m_cube_depth) / 2.0f);
    
    for (uint32_t row = 0; row < m_cube_row; ++row) {
        for (uint32_t col = 0; col < m_cube_col; ++col) {
            for (uint32_t depth = 0; depth < m_cube_depth; ++depth) {
                EntityWeakReferenceObject cube_node = ECSManager::GetRef().CreateEntity(StringFormat("cube_%d_%d_%d", row, col, depth));
                m_entities.push_back(cube_node);
                ECSManager::GetRef().AddComponentForEntity<TransformComponent>(cube_node, StringFormat("cube_%d_%d_%d_Transform", row, col, depth));
                ECSManager::GetRef().AddComponentForEntity<MeshRenderComponent>(cube_node, StringFormat("cube_%d_%d_%d_MeshRender", row, col, depth));
                SD_TYPE_COMP_WREF(m_scene_root_node, TransformComponent).AddChild(SD_GET_TYPE_COMP_WREF(cube_node, TransformComponent));

                SD_TYPE_COMP_WREF(cube_node, MeshRenderComponent).Initialize();
                SD_TYPE_COMP_WREF(cube_node, MeshRenderComponent).AppendMesh(m_cube_mesh, m_basic_material);
                SD_TYPE_COMP_WREF(cube_node, TransformComponent).SetWorldPosition(start_pos +
                    Vector3f((m_cube_side_length + m_cube_interval) * row,
                             (m_cube_side_length + m_cube_interval) * col,
                             (m_cube_side_length + m_cube_interval) * depth));
            }
        }
    }

    return result;
}

bool SampleDrawObjects::Unload()
{
    bool result = Scene::Unload();
    if (result == false) {
        return false;
    }
    m_axis_shader.Reset();
    m_axis_mesh.Reset();
    m_axis_material.Reset();
    m_basic_material.Reset();
    m_main_tex.Reset();
    return true;
}