/*==============  SD Engine License ==============
MIT License

Copyright (c) 2019 Tai-Yang, Chen

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

#include "GraphicsCompositor.h"
#include "LogManager.h"
#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

GraphicsCompositor::GraphicsCompositor(const ObjectName &i_name, const GraphicsSwapchainWeakReferenceObject &i_swapchain)
: Object(i_name)
, m_swapchain(i_swapchain)
, m_last_slot_id(0)
, m_current_slot_id(0)
, m_frame_count(0)
, m_tex_valids{false}
{
}

GraphicsCompositor::~GraphicsCompositor()
{
}

void GraphicsCompositor::Initialize()
{
	RenderPassWeakReferenceObject comp_rp = GraphicsManager::GetRef().GetRenderPass(sRenderPass_Compositing);
	if (m_swapchain.IsNull() == false && comp_rp.IsNull() == false) {
		//1. get swapchain size.
		ClearValue clear_color;
		clear_color.color.float32[0] = 0.0f;
		clear_color.color.float32[1] = 0.0f;
		clear_color.color.float32[2] = 0.0f;
		clear_color.color.float32[3] = 0.0f;
		Resolution resolution = SD_WREF(m_swapchain).GetResolution();
		//2. create command buffer.
		m_cmd_pools = new CommandPool("GCCmdPool");
		SD_REF(m_cmd_pools).Initialize();
		m_cmd_buffer = SD_REF(m_cmd_pools).AllocateCommandBuffer("GCCmdBuffer");
		//3. create textures for swapchain.
		uint32_t tex_number = SD_WREF(m_swapchain).GetTextureSize();
		m_compose_texs.resize(tex_number);
		m_flows.resize(tex_number);
		m_tex_valids.resize(tex_number);
		for (uint32_t i = 0; i < tex_number; ++i) {
			m_compose_texs[i] = new Texture(SDE::Basic::StringFormat("GCTex_%d", i));
			SD_SREF(m_compose_texs[i]).Initialize2DColorOrDepthBuffer(
				resolution.GetWidth(), resolution.GetHeight(),
				GraphicsManager::GetRef().GetDefaultColorBufferFormat(),
				ImageLayout_COLOR_ATTACHMENT_OPTIMAL);

			m_flows[i] = new RenderFlow(SDE::Basic::StringFormat("RenderFlow_%d", i), ImageOffset(0, 0, 0), ImageSize(resolution.GetWidth(), resolution.GetHeight(), 1));
			SD_SREF(m_flows[i]).RegisterRenderPass(comp_rp);
			SD_SREF(m_flows[i]).AllocateFrameBuffer();
			SD_SREF(m_flows[i]).RegisterBufferToFrameBuffer(m_compose_texs[i], 0, clear_color);
			SD_SREF(m_flows[i]).Initialize();

			m_tex_valids[i] = false;
		}
		m_last_slot_id = 0;
		m_current_slot_id = 0;
		m_frame_count = 0;
		//4. create descriptor set and uniform buffer.
		std::map<ObjectName, UniformVariableWeakReferenceObject> uvs;
		uint32_t desc_counts[UniformBindingType_MAX_DEFINE_VALUE] = { 0 };

		//1. Get DescriptorSet for MeshRenderer and count number of uniforms.
		DescriptorSetLayoutWeakReferenceObject basic_ds_layouts = GraphicsManager::GetRef().GetBasicDescriptorSetLayout(sUniformBuffer_Compositing_UVData);
		SD_WREF(basic_ds_layouts).GetUniformDescriptorCounts(desc_counts);

		//2. Allocate descriptor pool and set.
		m_dp = new DescriptorPool("DescriptorSetPool");
		SD_SREF(m_dp).Initialize(desc_counts, 1, false);
		m_ds = SD_SREF(m_dp).AllocateDescriptorSet(basic_ds_layouts);
		SD_WREF(m_ds).GetAllocatedUniformVariables(uvs);

		//3. Write descriptor to GPU for this set.
		if (m_ds.IsNull() == false) {
			SD_WREF(m_ds).WriteDescriptor();
		}

		//4. Get basic uniform buffer for update MVP.
		if (uvs.find(sUniformBuffer_Compositing_UVData) != uvs.end()) {
			m_ub = uvs[sUniformBuffer_Compositing_UVData].DynamicCastTo<UniformBuffer>();
		}

		if (m_ub.IsNull() == true) {
			SDLOGE("We can find geometry uniform buffer.");
		}
	}
}

void GraphicsCompositor::Render(const std::vector<TextureWeakReferenceObject> &i_tex)
{
	//find ready slot.

	//render to ready slot id.

}

TextureWeakReferenceObject GraphicsCompositor::GetValidTexture()
{
	if (m_tex_valids[m_last_slot_id] == false) {
		return m_compose_texs[m_last_slot_id];
	}
	else {
		return TextureWeakReferenceObject();
	}
}

______________SD_END_GRAPHICS_NAMESPACE______________