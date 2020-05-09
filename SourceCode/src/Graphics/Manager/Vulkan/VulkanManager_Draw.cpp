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