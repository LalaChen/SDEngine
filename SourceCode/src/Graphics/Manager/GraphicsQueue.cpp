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

#include "GraphicsQueue.h"
#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

GraphicsQueue::GraphicsQueue(const ObjectName &i_name, QueueAbility i_abilities, CompHandle i_phy_device, CompHandle i_device, uint32_t i_q_fam_id, uint32_t i_q_id)
: Object(i_name)
{
	m_identity.m_abilities  = i_abilities;
	m_identity.m_family_id  = i_q_fam_id;
	m_identity.m_queue_id   = i_q_id;
	m_identity.m_phy_device = i_phy_device;
	m_identity.m_device     = i_device;
}

GraphicsQueue::~GraphicsQueue()
{
	GraphicsManager::GetRef().DestroyQueue(m_identity);
}

void GraphicsQueue::Initialize()
{
	GraphicsManager::GetRef().CreateQueue(m_identity);

	m_fence = new GraphicsFence(m_object_name + "_Fence", m_identity.m_device);
	SD_SREF(m_fence).Initialize();
}

uint32_t GraphicsQueue::GetQueueID() const
{
	return m_identity.m_queue_id;
}

uint32_t GraphicsQueue::GetQueueFamilyID() const
{
	return m_identity.m_family_id;
}

void GraphicsQueue::SubmitCommandBuffer(const CommandBufferWeakReferenceObject &i_cmd_buffer)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	GraphicsManager::GetRef().SubmitCommandBuffersToQueue(m_identity, m_fence, {i_cmd_buffer});
}

void GraphicsQueue::SubmitCommandBuffers(const std::vector<CommandBufferWeakReferenceObject> &i_cmd_buffers)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	GraphicsManager::GetRef().SubmitCommandBuffersToQueue(m_identity, m_fence, i_cmd_buffers);
}

void GraphicsQueue::Present(const GraphicsSwapchainIdentity &i_sw_identity, uint32_t image_id, const std::vector<GraphicsSemaphoreWeakReferenceObject> &i_semas)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	GraphicsManager::GetRef().PresentToQueue(m_identity, i_sw_identity, image_id, i_semas);
}

______________SD_END_GRAPHICS_NAMESPACE______________