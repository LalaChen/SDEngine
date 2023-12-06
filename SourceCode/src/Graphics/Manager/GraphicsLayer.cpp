#include "GraphicsLayer.h"

#include "LogManager.h"
#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

GraphicsLayer::GraphicsLayer(const ObjectName &i_object_name)
: Object(i_object_name)
, m_region(0.0f, 0.0f, 1.0f, 1.0f)
, m_layer_order(UINT32_MAX)
{
}
GraphicsLayer::~GraphicsLayer()
{
	m_dp.reset();
}

void GraphicsLayer::RegisterTexture(const TextureWeakReferenceObject &i_color_tex, const TextureWeakReferenceObject &i_depth_tex, uint32_t i_order)
{
	m_color_texture = i_color_tex;
	m_depth_texture = i_depth_tex;
	m_layer_order = i_order;
}

uint32_t GraphicsLayer::GetLayerOrder() const
{
	return m_layer_order;
}

void GraphicsLayer::Initialize()
{
	//1. Allocate descriptor pool and set.

	std::map<ObjectName, UniformVariableWeakReferenceObject> uvs;
	uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };

	DescriptorSetLayoutWeakReferenceObject basic_ds_layouts = GraphicsManager::GetRef().GetBasicDescriptorSetLayout(sUniformDescriptorSetLayout_Compositing);
	SD_WREF(basic_ds_layouts).GetUniformDescriptorCounts(desc_counts);

	m_dp = new DescriptorPool("DescriptorSetPool");
	SD_SREF(m_dp).Initialize(desc_counts, 1, false);
	m_ds = SD_SREF(m_dp).AllocateDescriptorSet(basic_ds_layouts);
	SD_WREF(m_ds).GetAllocatedUniformVariables(uvs);

	//2. Get basic uniform buffer for update MVP.
	if (uvs.find(sUniformBuffer_Compositing_UVData) != uvs.end()) {
		m_ub = uvs[sUniformBuffer_Compositing_UVData].DynamicCastTo<UniformBuffer>();
	}

	if (m_ub.IsNull() == true) {
		SDLOGE("We can find uniform buffer.");
	}

	if (uvs.find(sUniformImage_Compositing_Texture) != uvs.end()) {
		m_ui = uvs[sUniformImage_Compositing_Texture].DynamicCastTo<UniformImages>();
		SD_WREF(m_ui).SetTexture(m_color_texture, 0);
	}

	if (m_ui.IsNull() == true) {
		SDLOGE("We can find uniform image.");
	}

	m_layer_sp = GraphicsManager::GetRef().GetShaderProgram("LayerComposingShader");
	m_layer_rp = GraphicsManager::GetRef().GetRenderPass(sRenderPass_Composing);

	RefreshRenderRegion(m_region);

	//3. Write descriptor to GPU for this set.
	if (m_ds.IsNull() == false) {
		SD_WREF(m_ds).WriteDescriptor();
	}

}

void GraphicsLayer::RefreshRenderRegion(const Vector3f &i_region)
{
	if (m_ub.IsNull() == false) {
		SD_WREF(m_ub).SetVector3f(sUniformBuffer_Compositing_UVData, i_region);
		SD_WREF(m_ub).Update();
	}
}

void GraphicsLayer::BindUniformVariables(const CommandBufferWeakReferenceObject &i_cb)
{
	SD_WREF(m_layer_sp).UseProgramWithTargetDescriptorSet(i_cb, m_layer_rp, 0, 0, { m_ds });
}

______________SD_END_GRAPHICS_NAMESPACE______________