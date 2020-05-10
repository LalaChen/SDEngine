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
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::SetViewport(const CommandBufferWeakReferenceObject &i_cb_wref, const Viewport &i_vp)
{
    const CommandBufferIdentity &cb_identity = GetIdentity(i_cb_wref);
    VkCommandBuffer cb_handle = reinterpret_cast<VkCommandBuffer>(cb_identity.m_handle);
    VkViewport vp;
    vp.x        = i_vp.m_x;
    vp.y        = i_vp.m_y;
    vp.width    = i_vp.m_width;
    vp.height   = i_vp.m_height;
    vp.minDepth = i_vp.m_min_depth;
    vp.maxDepth = i_vp.m_max_depth;
    SetVkViewport(cb_handle, vp);
}

void VulkanManager::SetViewports(const CommandBufferWeakReferenceObject& i_cb_wref, const std::vector<Viewport> &i_vps)
{
    const CommandBufferIdentity &cb_identity = GetIdentity(i_cb_wref);
    VkCommandBuffer cb_handle = reinterpret_cast<VkCommandBuffer>(cb_identity.m_handle);
    std::vector<VkViewport> vps;
    vps.resize(i_vps.size());
    for (uint32_t idx = 0; idx < vps.size(); ++idx) {
        vps[idx].x        = i_vps[idx].m_x;
        vps[idx].y        = i_vps[idx].m_y;
        vps[idx].width    = i_vps[idx].m_width;
        vps[idx].height   = i_vps[idx].m_height;
        vps[idx].minDepth = i_vps[idx].m_min_depth;
        vps[idx].maxDepth = i_vps[idx].m_max_depth;
    }
    SetVkViewports(cb_handle, vps);
}

void VulkanManager::SetScissor(const CommandBufferWeakReferenceObject &i_cb_wref, const ScissorRegion &i_region)
{
    const CommandBufferIdentity &cb_identity = GetIdentity(i_cb_wref);
    VkCommandBuffer cb_handle = reinterpret_cast<VkCommandBuffer>(cb_identity.m_handle);
    VkRect2D rect;
    rect.offset.x = i_region.m_x;
    rect.offset.y = i_region.m_y;
    rect.extent.width = i_region.m_width;
    rect.extent.height = i_region.m_height;
    SetVkScissor(cb_handle, rect);
}

void VulkanManager::SetScissors(const CommandBufferWeakReferenceObject &i_cb_wref, const std::vector<ScissorRegion> &i_regions)
{
    const CommandBufferIdentity &cb_identity = GetIdentity(i_cb_wref);
    VkCommandBuffer cb_handle = reinterpret_cast<VkCommandBuffer>(cb_identity.m_handle);
    std::vector<VkRect2D> rects;
    rects.resize(i_regions.size());
    for (uint32_t idx = 0; idx < i_regions.size(); ++idx) {
        rects[idx].offset.x      = i_regions[idx].m_x;
        rects[idx].offset.y      = i_regions[idx].m_y;
        rects[idx].extent.width  = i_regions[idx].m_width;
        rects[idx].extent.height = i_regions[idx].m_height;
    }
    SetVkScissors(cb_handle, rects);
}

void VulkanManager::DrawByIndices(const IndexBufferWeakReferenceObject &i_ib_wref, const CommandBufferWeakReferenceObject &i_cb_wref, uint32_t i_first_id, int32_t i_offset, uint32_t i_first_ins_id, uint32_t i_ins_number)
{
    const CommandBufferIdentity &cb_identity = GetIdentity(i_cb_wref);
    const IndexBufferIdentity &ib_idnetity = GetIdentity(i_ib_wref);
    VkCommandBuffer cb_handle = reinterpret_cast<VkCommandBuffer>(cb_identity.m_handle);
    DrawByVkIndexBuffer(cb_handle, 
        ib_idnetity.m_index_array_size,
        i_ins_number,
        i_first_id,
        i_offset,
        i_first_ins_id);
}

______________SD_END_GRAPHICS_NAMESPACE______________