#include "SDEngine.h"
#include "SampleDrawObjects.h"

using namespace SDE::Math;
using namespace SDE::Basic;
using namespace SDE::Graphics;

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

//#define INITIAL_BY_CODE
bool SampleDrawObjects::Load()
{
    bool result = Scene::Load();
    if (result == false) {
        SDLOGW("Load scene failure.");
        return result;
    }
    //1. allocate shader programs.
#if defined(INITIAL_BY_CODE)
    //-------------------- AxesShaderWithTexture ---------------------------
    {
        //1.1 axis_shader.
        m_axis_shader = new ShaderProgram("Axis");

        ShaderModules shader_modules;
        ShaderModuleStrongReferenceObject vert_shader = new ShaderModule("AxesShaderVert");
        vert_shader.GetRef().LoadBinaryShader(ShaderKind_VERTEX, "Shader/AxesShader.vert.spv", "main");
        ShaderModuleStrongReferenceObject frag_shader = new ShaderModule("AxesShaderFrag");
        frag_shader.GetRef().LoadBinaryShader(ShaderKind_FRAGMENT, "Shader/AxesShader.frag.spv", "main");
        shader_modules.m_shaders[ShaderKind_VERTEX] = vert_shader;
        shader_modules.m_shaders[ShaderKind_FRAGMENT] = frag_shader;

        //1.2 write pipeline parames.
        GraphicsPipelineParam params;
        params.m_primitive_info.m_primitive = Primitive_TRIANGLE;
        params.m_depth_stencil_info.m_depth_test_enable = true;
        params.m_rasterization_info.m_face_culling = FaceCulling_BACK_FACE;
        params.m_attachment_blend_state.m_blend_infos.resize(1); //blend default false.
        params.m_dynamic_states.push_back(DynamicState_VIEWPORT);
        params.m_dynamic_states.push_back(DynamicState_SCISSOR);
        GraphicsManager::GetRef().GetBasicVertexAttribInfos(params.m_va_binding_descs, params.m_va_location_descs, 2);

        //1.3 prepare descriptor set layouts.
        std::vector<DescriptorSetLayoutWeakReferenceObject> dsls;
        std::vector<DescriptorSetLayoutWeakReferenceObject> basic_dsls;
        
        dsls.push_back(GraphicsManager::GetRef().GetBasicDescriptorSetLayout("Camera"));
        dsls.push_back(GraphicsManager::GetRef().GetBasicDescriptorSetLayout("MeshRender"));
        dsls.push_back(GraphicsManager::GetRef().GetBasicDescriptorSetLayout("Light"));

        basic_dsls.push_back(GraphicsManager::GetRef().GetBasicDescriptorSetLayout("Camera"));
        basic_dsls.push_back(GraphicsManager::GetRef().GetBasicDescriptorSetLayout("MeshRender"));
        basic_dsls.push_back(GraphicsManager::GetRef().GetBasicDescriptorSetLayout("Light"));

        //1.4 register uniform variable descriptor to pipeline.
        RenderPassWeakReferenceObject rp = GraphicsManager::GetRef().GetRenderPass("ForwardPass");
        GraphicsPipelineStrongReferenceObject pipeline = new GraphicsPipeline("AxesShader_Forward");
        pipeline.GetRef().SetGraphicsPipelineParams(params, rp, dsls, 0);
        pipeline.GetRef().Initialize(shader_modules);

        //1.5 prepare datas and then initalize shader structure.
        std::vector<GraphicsPipelineStrongReferenceObject> pipes;
        pipes.push_back(pipeline);

        ShaderProgram::RenderPassInfos rp_infos;
        RenderPassInfo forward_rp;
        forward_rp.m_rp = rp;
        RenderSubPassPipelineInfo rsp_info;
        RenderStepInfo rs_info;
        rs_info.m_dsls = dsls;
        rs_info.m_pipe = pipeline;
        rsp_info.m_step_infos.push_back(rs_info);
        forward_rp.m_sp_pipe_infos.push_back(rsp_info); //use pipeline 0 at sp0.
        rp_infos.push_back(forward_rp);
        m_axis_shader.GetRef().RegisterShaderProgramStructure(
            rp_infos, basic_dsls, {}/*{ material_dsl }*/);
    }
    //2. add texture.
    {
        m_main_tex = new Texture("MainTexture");
        m_main_tex.GetRef().SetSamplerFilterType(SamplerFilterType_LINEAR, SamplerFilterType_LINEAR);
        m_main_tex.GetRef().InitializeFromImageResource("Texture/Lenna.png");
    }

    //3. allocate material.
    {
        m_axis_material = new Material("AxesMaterial");
        m_axis_material.GetRef().BindShaderProgram(m_axis_shader);
        //Set data done. Link with shader program.(Write descirptor)
        m_axis_material.GetRef().LinkWithShaderProgram();
        MaterialUniforms mat_ub; //use default color.
        m_axis_material.GetRef().SetDataToUniformBuffer("material", &mat_ub, sizeof(MaterialUniforms));
        m_axis_material.GetRef().Update();
    }

    {
        m_basic_material = new Material("BasicMaterial");
        m_basic_material.GetRef().BindShaderProgram(GraphicsManager::GetRef().GetShaderProgram("BasicShading"));
        //Set data done. Link with shader program.(Write descirptor)
        m_basic_material.GetRef().LinkWithShaderProgram();
        MaterialUniforms mat_ub; //use default color.
        m_basic_material.GetRef().SetDataToUniformBuffer("material", &mat_ub, sizeof(MaterialUniforms));
        m_basic_material.GetRef().SetTexture("mainTexture", m_main_tex);
        m_basic_material.GetRef().Update();
    }
#else
    {
        GraphicsManager::GetRef().RegisterShaderProgram("AxesShader", "ShaderProgram/AxesShading/AxesShading.json");
        GraphicsManager::GetRef().RegisterShaderProgram("BasicShader", "ShaderProgram/BasicShading/BasicShading.json");
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
        m_axis_material.GetRef().SetDataToUniformBuffer("material", &mat_ub, sizeof(MaterialUniforms));
        m_axis_material.GetRef().Update();
    }
    {
        m_basic_material = new Material("BasicMaterial");
        m_basic_material.GetRef().BindShaderProgram(GraphicsManager::GetRef().GetShaderProgram("BasicShader"));
        //Set data done. Link with shader program.(Write descirptor)
        m_basic_material.GetRef().LinkWithShaderProgram();
        MaterialUniforms mat_ub; //use default color.
        m_basic_material.GetRef().SetDataToUniformBuffer("material", &mat_ub, sizeof(MaterialUniforms));
        m_basic_material.GetRef().SetTexture("textures", m_main_tex, 0);
        m_basic_material.GetRef().Update();
    }
#endif

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

    //6. Add light.
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

    //7. add axis.
    m_axis_mesh = BasicShapeCreator::GetRef().CreateAxis(0.2f, 20.0f);

    m_axis_node = ECSManager::GetRef().CreateEntity("AxisNode");
    m_entities.push_back(m_axis_node);

    ECSManager::GetRef().AddComponentForEntity<TransformComponent>(m_axis_node, "AxisTransform");
    ECSManager::GetRef().AddComponentForEntity<MeshRenderComponent>(m_axis_node, "AxisMeshRender");
    SD_TYPE_COMP_WREF(m_scene_root_node, TransformComponent).AddChild(SD_GET_TYPE_COMP_WREF(m_axis_node, TransformComponent));

    SD_TYPE_COMP_WREF(m_axis_node, TransformComponent).SetWorldPosition(Vector3f(0.0f, 0.0001f, 0.0f));
    SD_TYPE_COMP_WREF(m_axis_node, MeshRenderComponent).Initialize();
    SD_TYPE_COMP_WREF(m_axis_node, MeshRenderComponent).AppendMesh(m_axis_mesh, m_axis_material);

    //8. add floor.
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

    //8. add cubes.
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