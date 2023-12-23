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

#include "VulkanQueueFamily.h"
#include "LogManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

VulkanQueueFamily::VulkanQueueFamily()
: m_support_surface(0)
, m_family_id(0xFFFFFFFF)
{
}

VulkanQueueFamily::~VulkanQueueFamily()
{
}

bool VulkanQueueFamily::IsSuitable(VkQueueFlags i_flags) const
{
    return ((m_prop.queueFlags & i_flags) == i_flags);
}

void VulkanQueueFamily::Initialize(
    VkQueueFamilyProperties i_prop,
    uint32_t i_id, uint32_t i_queue_number,
    bool i_support_surface)
{
    m_prop = i_prop;
    m_family_id = i_id;
    m_support_surface = i_support_surface;

    uint32_t final_number = i_queue_number;
    if (i_queue_number >= m_prop.queueCount) {
        final_number = m_prop.queueCount;
    }

    m_queue_states = std::vector<bool>(final_number, false);
    m_priorities = std::vector<float>(final_number, 1.0f);

    SDLOG("Final Present Queue Family[%d] : TimeStamp[%d] Flag(%x) Count:%d Extent3D(%d,%d,%d) ",
        m_family_id,
        m_prop.timestampValidBits,
        m_prop.queueFlags,
        m_prop.queueCount,
        m_prop.minImageTransferGranularity.width,
        m_prop.minImageTransferGranularity.height,
        m_prop.minImageTransferGranularity.depth);
}

bool VulkanQueueFamily::IsSupportingSurface() const
{
    return m_support_surface;
}

uint32_t VulkanQueueFamily::GetQueueNumber() const
{
    return static_cast<uint32_t>(m_queue_states.size());
}

uint32_t VulkanQueueFamily::GetQueueFamilyID() const
{
    return m_family_id;
}

const float* VulkanQueueFamily::GetQueuePriorities() const
{
    return m_priorities.data();
}

int32_t VulkanQueueFamily::GetEmptyQueue() const
{
    std::lock_guard<std::mutex> lock(m_state_mutex);
    for (uint32_t qid = 0; qid < m_queue_states.size(); ++qid) {
        if (m_queue_states[qid] == false) {
            return qid;
        }
    }
    return -1;
}

bool VulkanQueueFamily::RequestQueue(int32_t i_qid)
{
    std::lock_guard<std::mutex> lock(m_state_mutex);
    if (i_qid >= 0 && i_qid < m_queue_states.size()) {
        if (m_queue_states[i_qid] == false) {
            m_queue_states[i_qid] = true;
            return true;
        }
        else {
            SDLOGE("Queue ID(%d) in family(%d) is used.", i_qid, m_family_id);
            return false;
        }
    }
    else {
        SDLOGE("Queue ID(%d) is out of range for family(%d).", i_qid, m_family_id);
        return false;
    }
}

bool VulkanQueueFamily::ReturnQueue(int32_t i_qid)
{
    std::lock_guard<std::mutex> lock(m_state_mutex);
    if (i_qid >= 0 && i_qid < m_queue_states.size()) {
        if (m_queue_states[i_qid] == true) {
            m_queue_states[i_qid] = false;
            return true;
        }
        else {
            SDLOGE("Queue ID(%d) in family(%d) is on used.", i_qid, m_family_id);
            return false;
        }
    }
    else {
        SDLOGE("Queue ID(%d) is out of range for family(%d).", i_qid, m_family_id);
        return false;
    }
}

void VulkanQueueFamily::SetQueuePriority(int32_t i_qid, float i_priority)
{
    if (i_qid >= 0 && i_qid < m_queue_states.size()) {
        if (m_queue_states[i_qid] == true) {
            m_queue_states[i_qid] = false;
        }
    }
    else {
        SDLOGE("Queue ID(%d) is out of range for family(%d).", i_qid, m_family_id);
    }
}


______________SD_END_GRAPHICS_NAMESPACE______________