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

/*! \file      VulkanFamily.h
 *  \brief     Introduce of class VulkanFamily
 *  \author    Kuan-Chih, Chen
 *  \date      2022/06/25
 *  \copyright MIT License.
 */

#include <mutex>

#include "SDEngineMacro.h"
#include "SDEngineCommonFunction.h"
#include "VulkanWrapper.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

class SDENGINE_CLASS VulkanQueueFamily
{
public:
    VulkanQueueFamily();
    ~VulkanQueueFamily();
public:
    void Initialize(
        VkQueueFamilyProperties i_prop,
        uint32_t i_id,
        uint32_t i_queue_number,
        bool i_support_surface);
public:
    bool IsSuitable(VkQueueFlags i_flags) const;
    bool IsSupportingSurface() const;
    int32_t GetEmptyQueue() const;
    bool RequestQueue(int32_t i_qid);
    bool ReturnQueue(int32_t i_qid);
    void SetQueuePriority(int32_t i_qid, float i_priority);
    uint32_t GetQueueNumber() const;
    uint32_t GetQueueFamilyID() const;
    const float* GetQueuePriorities() const;
public:
    VkQueueFamilyProperties m_prop;
    std::vector<bool> m_queue_states;
    std::vector<float> m_priorities;
    uint32_t m_family_id;
    bool m_support_surface;
public:
    mutable std::mutex m_state_mutex;
};


______________SD_END_GRAPHICS_NAMESPACE______________
