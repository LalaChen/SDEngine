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

const ShaderModuleIdentity& GraphicsManager::GetIdentity(const ShaderModuleWeakReferenceObject &i_module) const
{
    return SD_CONST_WREF(i_module).m_identity;
}

const TextureIdentity& GraphicsManager::GetIdentity(const TextureWeakReferenceObject &i_tex) const
{
    return SD_CONST_WREF(i_tex).m_tex_identity;
}

const SamplerIdentity& GraphicsManager::GetIdentityFromTexture(const TextureWeakReferenceObject &i_tex) const
{
    return SD_CONST_WREF(i_tex).m_sampler_idnetity;
}

const FrameBufferIdentity& GraphicsManager::GetIdentity(const FrameBufferWeakReferenceObject &i_fb) const
{
    return SD_CONST_WREF(i_fb).m_identity;
}

const CommandBufferIdentity& GraphicsManager::GetIdentity(const CommandBufferWeakReferenceObject &i_cb) const
{
    return SD_CONST_WREF(i_cb).m_identity;
}

const CommandPoolIdentity& GraphicsManager::GetIdentity(const CommandPoolWeakReferenceObject &i_cp) const
{
    return SD_CONST_WREF(i_cp).m_identity;
}

const RenderPassIdentity& GraphicsManager::GetIdentity(const RenderPassWeakReferenceObject &i_rp) const
{
    return SD_CONST_WREF(i_rp).m_identity;
}

const VertexBufferIdentity& GraphicsManager::GetIdentity(const VertexBufferWeakReferenceObject &i_vb) const
{
    return SD_CONST_WREF(i_vb).m_identity;
}

const IndexBufferIdentity& GraphicsManager::GetIdentity(const IndexBufferWeakReferenceObject &i_ib) const
{
    return SD_CONST_WREF(i_ib).m_identity;
}

const UniformBufferIdentity& GraphicsManager::GetIdentity(const UniformBufferWeakReferenceObject &i_ub) const
{
    return SD_CONST_WREF(i_ub).m_identity;
}

const GraphicsPipelineIdentity& GraphicsManager::GetIdentity(const GraphicsPipelineWeakReferenceObject &i_pipe) const
{
    return SD_CONST_WREF(i_pipe).m_identity;
}

const DescriptorPoolIdentity& GraphicsManager::GetIdentity(const DescriptorPoolWeakReferenceObject &i_pool) const
{
    return SD_CONST_WREF(i_pool).m_identity;
}

const DescriptorSetIdentity& GraphicsManager::GetIdentity(const DescriptorSetWeakReferenceObject &i_desc) const
{
    return SD_CONST_WREF(i_desc).m_identity;
}

const DescriptorSetLayoutIdentity& GraphicsManager::GetIdentity(const DescriptorSetLayoutWeakReferenceObject &i_ds_layout) const
{
    return SD_CONST_WREF(i_ds_layout).m_identity;
}

______________SD_END_GRAPHICS_NAMESPACE______________