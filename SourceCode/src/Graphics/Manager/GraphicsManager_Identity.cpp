/*==============  SD Engine License ==============
MIT License

Copyright (c) 2019 Kuan-Chih, Chen

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

#include "LogManager.h"
#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

const ShaderModuleIdentity& GraphicsManager::GetIdentity(const ShaderModuleWeakReferenceObject &i_module_wref) const
{
    return SD_CONST_WREF(i_module_wref).m_identity;
}

const TextureIdentity& GraphicsManager::GetIdentity(const TextureWeakReferenceObject &i_tex_wref) const
{
    return SD_CONST_WREF(i_tex_wref).m_tex_identity;
}

const SamplerIdentity& GraphicsManager::GetIdentityFromTexture(const TextureWeakReferenceObject &i_tex_wref) const
{
    return SD_CONST_WREF(i_tex_wref).m_sampler_idnetity;
}

const FrameBufferIdentity& GraphicsManager::GetIdentity(const FrameBufferWeakReferenceObject &i_fb_wref) const
{
    return SD_CONST_WREF(i_fb_wref).m_identity;
}

const CommandBufferIdentity& GraphicsManager::GetIdentity(const CommandBufferWeakReferenceObject &i_cb_wref) const
{
    return SD_CONST_WREF(i_cb_wref).m_identity;
}

const CommandPoolIdentity& GraphicsManager::GetIdentity(const CommandPoolWeakReferenceObject &i_cp_wref) const
{
    return SD_CONST_WREF(i_cp_wref).m_identity;
}

const RenderPassIdentity& GraphicsManager::GetIdentity(const RenderPassWeakReferenceObject &i_rp_wref) const
{
    return SD_CONST_WREF(i_rp_wref).m_identity;
}

const VertexBufferIdentity& GraphicsManager::GetIdentity(const VertexBufferWeakReferenceObject &i_vb_wref) const
{
    return SD_CONST_WREF(i_vb_wref).m_identity;
}

const IndexBufferIdentity& GraphicsManager::GetIdentity(const IndexBufferWeakReferenceObject &i_ib_wref) const
{
    return SD_CONST_WREF(i_ib_wref).m_identity;
}

const UniformBufferIdentity& GraphicsManager::GetIdentity(const UniformBufferWeakReferenceObject &i_ub_wref) const
{
    return SD_CONST_WREF(i_ub_wref).m_identity;
}

const GraphicsPipelineIdentity& GraphicsManager::GetIdentity(const GraphicsPipelineWeakReferenceObject &i_pipe_wref) const
{
    return SD_CONST_WREF(i_pipe_wref).m_identity;
}

const DescriptorPoolIdentity& GraphicsManager::GetIdentity(const DescriptorPoolWeakReferenceObject &i_pool_wref) const
{
    return SD_CONST_WREF(i_pool_wref).m_identity;
}

const DescriptorSetIdentity& GraphicsManager::GetIdentity(const DescriptorSetWeakReferenceObject &i_desc_wref) const
{
    return SD_CONST_WREF(i_desc_wref).m_identity;
}

const DescriptorSetLayoutIdentity& GraphicsManager::GetIdentity(const DescriptorSetLayoutWeakReferenceObject &i_ds_layout_wref) const
{
    return SD_CONST_WREF(i_ds_layout_wref).m_identity;
}

______________SD_END_GRAPHICS_NAMESPACE______________