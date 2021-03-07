#include "SDEngine.h"
#include "SampleDrawObjects.h"

using namespace SDE::Math;
using namespace SDE::Basic;
using namespace SDE::Graphics;

SampleDrawObjects::SampleDrawObjects(const ObjectName &i_sample_name)
: Sample(i_sample_name)
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

void SampleDrawObjects::Initialize()
{
}

void SampleDrawObjects::Update()
{
}

void SampleDrawObjects::Destroy()
{
}

void SampleDrawObjects::Resize()
{
}

void SampleDrawObjects::InitializeScene()
{
    LoadScene();
}

void SampleDrawObjects::UpdateScene()
{
    if (m_camera_motor_wref.IsNull() == false) {
        SD_WREF(m_camera_motor_wref).Update();
    }
}

void SampleDrawObjects::DestroyScene()
{
}

void SampleDrawObjects::LoadScene()
{
    //1. allocate shader programs.
    //-------------------- AxesShaderWithTexture ---------------------------
    {
        //1.1 axis_shader.
        m_axis_shader_sref = new ShaderProgram("Axis");

        ShaderModules shader_modules;
        ShaderModuleStrongReferenceObject vert_shader_sref = new ShaderModule("AxesShaderVert");
        vert_shader_sref.GetRef().LoadBinaryShader(ShaderKind_VERTEX, "Shader/AxesShader.vert.spv", "main");
        ShaderModuleStrongReferenceObject frag_shader_sref = new ShaderModule("AxesShaderFrag");
        frag_shader_sref.GetRef().LoadBinaryShader(ShaderKind_FRAGMENT, "Shader/AxesShader.frag.spv", "main");
        shader_modules.m_shaders[ShaderKind_VERTEX] = vert_shader_sref;
        shader_modules.m_shaders[ShaderKind_FRAGMENT] = frag_shader_sref;

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
        std::vector<DescriptorSetLayoutWeakReferenceObject> dsl_wrefs;
        std::vector<DescriptorSetLayoutWeakReferenceObject> basic_dsl_wrefs;
        
        dsl_wrefs.push_back(GraphicsManager::GetRef().GetBasicDescriptorSetLayout("Camera"));
        dsl_wrefs.push_back(GraphicsManager::GetRef().GetBasicDescriptorSetLayout("MeshRender"));
        dsl_wrefs.push_back(GraphicsManager::GetRef().GetBasicDescriptorSetLayout("Light"));

        basic_dsl_wrefs.push_back(GraphicsManager::GetRef().GetBasicDescriptorSetLayout("Camera"));
        basic_dsl_wrefs.push_back(GraphicsManager::GetRef().GetBasicDescriptorSetLayout("MeshRender"));
        basic_dsl_wrefs.push_back(GraphicsManager::GetRef().GetBasicDescriptorSetLayout("Light"));

        //1.4 register uniform variable descriptor to pipeline.
        RenderPassWeakReferenceObject rp_wref = GraphicsManager::GetRef().GetRenderPass("ForwardPath");
        GraphicsPipelineStrongReferenceObject pipeline_sref = new GraphicsPipeline("AxesShader_Forward");
        pipeline_sref.GetRef().SetGraphicsPipelineParams(params, rp_wref, dsl_wrefs, 0);
        pipeline_sref.GetRef().Initialize(shader_modules);

        //1.5 prepare datas and then initalize shader structure.
        std::vector<GraphicsPipelineStrongReferenceObject> pipe_srefs;
        pipe_srefs.push_back(pipeline_sref);

        ShaderProgram::RenderPassInfos rp_infos;
        RenderPassInfo forward_rp;
        forward_rp.m_rp_wref = rp_wref;
        RenderSubPassPipelineInfo rsp_info;
        rsp_info.m_dsl_wrefs = dsl_wrefs;
        rsp_info.m_pipe_id = 0;
        forward_rp.m_sp_pipe_infos.push_back(rsp_info); //use pipeline 0 at sp0.
        rp_infos.push_back(forward_rp);
        m_axis_shader_sref.GetRef().RegisterShaderProgramStructure(
            rp_infos, pipe_srefs, basic_dsl_wrefs, {}/*{ material_dsl_sref }*/);
    }
    //2. add texture.
    {
        m_main_tex_sref = new Texture("MainTexture");
        m_main_tex_sref.GetRef().SetSamplerFilterType(SamplerFilterType_LINEAR, SamplerFilterType_LINEAR);
        m_main_tex_sref.GetRef().InitializeFromImageResource("Texture/Lenna.png");
    }

    //3. allocate material.
    {
        m_axis_material_sref = new Material("AxesMaterial");
        m_axis_material_sref.GetRef().BindShaderProgram(m_axis_shader_sref);
        //Set data done. Link with shader program.(Write descirptor)
        m_axis_material_sref.GetRef().LinkWithShaderProgram();
        MaterialUniforms mat_ub; //use default color.
        m_axis_material_sref.GetRef().SetDataToUniformBuffer("material", &mat_ub, sizeof(MaterialUniforms));
        m_axis_material_sref.GetRef().Update();
    }

    {
        m_basic_material_sref = new Material("BasicMaterial");
        m_basic_material_sref.GetRef().BindShaderProgram(GraphicsManager::GetRef().GetShaderProgram("BasicShading"));
        //Set data done. Link with shader program.(Write descirptor)
        m_basic_material_sref.GetRef().LinkWithShaderProgram();
        MaterialUniforms mat_ub; //use default color.
        m_basic_material_sref.GetRef().SetDataToUniformBuffer("material", &mat_ub, sizeof(MaterialUniforms));
        m_basic_material_sref.GetRef().SetTexture("mainTexture", m_main_tex_sref);
        m_basic_material_sref.GetRef().Update();
    }

    //4. allocate scene root.
    m_scene_root_node = ECSManager::GetRef().CreateEntity("SceneRoot");
    ECSManager::GetRef().AddComponentForEntity<TransformComponent>(m_scene_root_node, "RootTransform");

    //5. Add camera.
    m_camera_node = ECSManager::GetRef().CreateEntity("Camera");
    ECSManager::GetRef().AddComponentForEntity<TransformComponent>(m_camera_node, "CameraTransform");
    ECSManager::GetRef().AddComponentForEntity<CameraComponent>(m_camera_node, "Camera");
    SD_COMP_WREF(m_scene_root_node, TransformComponent).AddChild(SD_GET_COMP_WREF(m_camera_node, TransformComponent));

    SD_COMP_WREF(m_camera_node, TransformComponent).SetWorldTransform(
        Transform::LookAt(
            Vector3f(1.0f, 1.5f, 7.0f, 1.0f),
            Vector3f(0.0f, 1.5f, 0.0f, 1.0f),
            Vector3f::PositiveY,
            true));

    SD_COMP_WREF(m_camera_node, CameraComponent).SetClearValues(
        { 0.35f, 0.35f, 0.75f, 1.0f },
        { 1.0f, 1 });
    SD_COMP_WREF(m_camera_node, CameraComponent).SetPerspective(120, 0.01f, 1000.0f);
    SD_COMP_WREF(m_camera_node, CameraComponent).Initialize();

    ECSManager::GetRef().AddComponentForEntity<MotorComponent>(m_camera_node, "CameraMotor");
    m_camera_motor_wref = SD_GET_COMP_WREF(m_camera_node, MotorComponent);
    SD_WREF(m_camera_motor_wref).Initialize();

    //6. Add light.
    m_light_node = ECSManager::GetRef().CreateEntity("Light");
    ECSManager::GetRef().AddComponentForEntity<TransformComponent>(m_light_node, "LightTransform");
    ECSManager::GetRef().AddComponentForEntity<LightComponent>(m_light_node, "Light");
    SD_COMP_WREF(m_scene_root_node, TransformComponent).AddChild(SD_GET_COMP_WREF(m_light_node, TransformComponent));
    SD_COMP_WREF(m_light_node, LightComponent).SetLightParameter(LightUniforms());
    SD_COMP_WREF(m_light_node, LightComponent).Initialize();

    SD_COMP_WREF(m_light_node, TransformComponent).SetWorldTransform(
        Transform::LookAt(Vector3f(1.0f, 3.0f, 1.0f, 1.0f), Vector3f::Origin, Vector3f::PositiveY)
    );

    //7. add axis.
    m_axis_mesh = BasicShapeCreator::GetRef().CreateAxis(0.2f, 20.0f);
    m_axis_node = ECSManager::GetRef().CreateEntity("AxisNode");
    ECSManager::GetRef().AddComponentForEntity<TransformComponent>(m_axis_node, "AxisTransform");
    ECSManager::GetRef().AddComponentForEntity<MeshRenderComponent>(m_axis_node, "AxisMeshRender");
    SD_COMP_WREF(m_scene_root_node, TransformComponent).AddChild(SD_GET_COMP_WREF(m_axis_node, TransformComponent));

    SD_COMP_WREF(m_axis_node, TransformComponent).SetWorldPosition(Vector3f(0.0f, 0.0001f, 0.0f));
    SD_COMP_WREF(m_axis_node, MeshRenderComponent).Initialize();
    SD_COMP_WREF(m_axis_node, MeshRenderComponent).AppendMesh(m_axis_mesh, m_axis_material_sref);

    //8. add floor.
    m_floor_mesh = BasicShapeCreator::GetRef().CreatePlane(
        Vector3f::Zero, Vector3f::PositiveZ, Vector3f::PositiveX,
        100.0f, 100.0f, 100.0f, 100.0f);
    m_floor_node = ECSManager::GetRef().CreateEntity("FloorNode");
    ECSManager::GetRef().AddComponentForEntity<TransformComponent>(m_floor_node, "FloorTransform");
    ECSManager::GetRef().AddComponentForEntity<MeshRenderComponent>(m_floor_node, "FloorMeshRender");
    SD_COMP_WREF(m_scene_root_node, TransformComponent).AddChild(SD_GET_COMP_WREF(m_floor_node, TransformComponent));

    SD_COMP_WREF(m_floor_node, MeshRenderComponent).Initialize();
    SD_COMP_WREF(m_floor_node, MeshRenderComponent).AppendMesh(m_floor_mesh, m_basic_material_sref);

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
                ECSManager::GetRef().AddComponentForEntity<TransformComponent>(cube_node, StringFormat("cube_%d_%d_%d_Transform", row, col, depth));
                ECSManager::GetRef().AddComponentForEntity<MeshRenderComponent>(cube_node, StringFormat("cube_%d_%d_%d_MeshRender", row, col, depth));
                SD_COMP_WREF(m_scene_root_node, TransformComponent).AddChild(SD_GET_COMP_WREF(cube_node, TransformComponent));

                SD_COMP_WREF(cube_node, MeshRenderComponent).Initialize();
                SD_COMP_WREF(cube_node, MeshRenderComponent).AppendMesh(m_cube_mesh, m_basic_material_sref);
                SD_COMP_WREF(cube_node, TransformComponent).SetWorldPosition(start_pos +
                    Vector3f((m_cube_side_length + m_cube_interval) * row,
                             (m_cube_side_length + m_cube_interval) * col,
                             (m_cube_side_length + m_cube_interval) * depth));
            }
        }
    }
}