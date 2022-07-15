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

/*! \file      GraphicsQueue.h
 *  \brief     Introduce of class GraphicsQueue
 *  \author    Kuan-Chih, Chen
 *  \date      2022/06/18
 *  \copyright MIT License.
 */

#pragma once

#include <mutex>

#include "SDEngineMacro.h"
#include "SDEngineCommonFunction.h"

#include "GraphicsQueueIdentity.h"
#include "GraphicsSwapchainIdentity.h"

#include "Object.h"
#include "CommandBuffer.h"
#include "GraphicsSynchronousElements.h"

using SDE::Basic::Object;
using SDE::Basic::ObjectName;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(GraphicsQueue);

class GraphicsQueue : public Object
{
public:
	friend class GraphicsIdentityGetter;
public:
	GraphicsQueue(const ObjectName &i_name, QueueAbility i_abilities, CompHandle i_phy_device, CompHandle i_device, uint32_t i_q_fam_id, uint32_t i_q_id);
	~GraphicsQueue();
public:
	void Initialize();
	void SubmitCommandBuffer(const CommandBufferWeakReferenceObject &i_cmd_buffer);
	void SubmitCommandBuffers(const std::vector<CommandBufferWeakReferenceObject> &i_cmd_buffers);
	void Present(const GraphicsSwapchainIdentity &i_sw_identity, uint32_t image_id, const std::vector<GraphicsSemaphoreWeakReferenceObject> &i_semas);
protected:
	GraphicsQueueIdentity m_identity;
	GraphicsFenceStrongReferenceObject m_fence;
protected:
	mutable std::mutex m_mutex;
};

______________SD_END_GRAPHICS_NAMESPACE______________