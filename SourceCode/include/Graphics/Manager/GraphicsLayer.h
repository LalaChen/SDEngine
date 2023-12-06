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

/*! \file      GraphicsLayer.h
 *  \brief     Introduce of class GraphicsLayer
 *  \author    Kuan-Chih, Chen
 *  \date      2023/11/25
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonFunction.h"

#include "Object.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "RenderPass.h"
#include "DescriptorPool.h"
#include "UniformImages.h"
#include "UniformBuffer.h"
#include "CommandBuffer.h"

using SDE::Basic::Object;
using SDE::Basic::ObjectName;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(GraphicsLayer);

class SDENGINE_CLASS GraphicsLayer : public Object
{
public:
	explicit GraphicsLayer(const ObjectName &i_object_name);
	virtual ~GraphicsLayer();
public:
	void RegisterTexture(const TextureWeakReferenceObject &i_color_tex, const TextureWeakReferenceObject &i_depth_tex, uint32_t i_order = 0);
	void Initialize();
	void RefreshRenderRegion(const Vector3f &i_region = Vector3f(0.0f, 0.0f, 1.0f, 1.0f));
	void BindUniformVariables(const CommandBufferWeakReferenceObject &i_cb);
	uint32_t GetLayerOrder() const;
protected:
	ShaderProgramWeakReferenceObject m_layer_sp;
	RenderPassWeakReferenceObject m_layer_rp;
	DescriptorPoolStrongReferenceObject m_dp;
	UniformBufferWeakReferenceObject m_ub;
	UniformImagesWeakReferenceObject m_ui;
	DescriptorSetWeakReferenceObject m_ds;
	TextureWeakReferenceObject m_color_texture;
	TextureWeakReferenceObject m_depth_texture;
	Vector3f m_region;
	uint32_t m_layer_order;
};

______________SD_END_GRAPHICS_NAMESPACE______________