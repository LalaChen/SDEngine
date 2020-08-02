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

/*! \file      Material.h
 *  \brief     Introduce of class Material.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/06/21
 *  \copyright MIT License.
 */

#pragma once

#include <vector>

#include "Transform.h"
#include "DescriptorPool.h"
#include "UniformImages.h"
#include "UniformBuffer.h"
#include "ShaderProgram.h"
#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;
using SDE::Math::Transform;
using SDE::Math::Matrix4X4f;
using SDE::Math::Vector3f;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Material);

/*! \class Material
 *  \brief In our graphic system, Material is used to be the bridge between data and pipeline.
 *         We keep data in material and used it when we update data into graphics card.
 */
class SDENGINE_CLASS Material : public Object
{
public:
    friend class GraphicsManager;
public:
    explicit Material(const ObjectName &i_object_name);
    virtual ~Material();
public:
    void BindShaderProgram(const ShaderProgramWeakReferenceObject& i_sp_wref);
public:
//uniform variable setter
    bool SetInt(const ObjectName &i_ub_name, const ObjectName &i_var_name, int32_t i_value, Size_ui32 i_idx = 0);
    
    bool SetUint(const ObjectName &i_ub_name, const ObjectName &i_var_name, uint32_t i_value, Size_ui32 i_idx = 0);
    
    bool SetFloat(const ObjectName &i_ub_name, const ObjectName &i_var_name, float i_value, Size_ui32 i_idx = 0);
    
    bool SetVector3f(const ObjectName &i_ub_name, const ObjectName &i_var_name, const Vector3f &i_value, Size_ui32 i_idx = 0);
    
    bool SetColor4f(const ObjectName &i_ub_name, const ObjectName &i_var_name, const Color4f &i_value, Size_ui32 i_idx = 0);
    
    bool SetMatrix4X4f(const ObjectName &i_ub_name, const ObjectName &i_var_name, const Matrix4X4f &i_value, Size_ui32 i_idx = 0);
    
public:
    bool SetIntArray(const ObjectName &i_ub_name, const std::string &i_var_name, const std::vector<int32_t> &i_datas, Size_ui32 i_start_idx = 0);

    bool SetUintArray(const ObjectName &i_ub_name, const std::string &i_var_name, const std::vector<uint32_t> &i_datas, Size_ui32 i_start_idx = 0);

    bool SetFloatArray(const ObjectName &i_ub_name, const std::string &i_var_name, const std::vector<float> &i_datas, Size_ui32 i_start_idx = 0);

    bool SetVector3fArray(const ObjectName &i_ub_name, const std::string &i_var_name, const std::vector<Vector3f> &i_datas, Size_ui32 i_start_idx = 0);

    bool SetColor4fArray(const ObjectName &i_ub_name, const std::string &i_var_name, const std::vector<Color4f> &i_datas, Size_ui32 i_start_idx = 0);

    bool SetMatrix4X4fArray(const ObjectName &i_ub_name, const std::string &i_var_name, const std::vector<Matrix4X4f> &i_datas, Size_ui32 i_start_idx = 0);
public:

    bool SetDataToUniformBuffer(const ObjectName &i_ub_name, const void *i_data, Size_ui32 i_data_size);
public:
    //Need to refresh write descriptor. So it maybe has extra cost.
    bool SetTexture(const ObjectName &i_uv_name, const TextureWeakReferenceObject &i_tex_wref, int32_t i_idx = 0);//Need to visit
public:
    void LinkWithShaderProgram();

    void RefreshLinkingWithShaderProgram();
public:
//bind shader to command buffer.
    void Update();
    void UseMaterial(const CommandBufferWeakReferenceObject &i_cb_wref, const RenderPassWeakReferenceObject &i_rp_wref, uint32_t i_sp_id);
protected:
    std::map<ObjectName, UniformVariableStrongReferenceObject> m_uv_srefs;
    ShaderProgramWeakReferenceObject m_sp_wref;
    DescriptorPoolStrongReferenceObject m_dsp_sref;
    /*! \var std::vector<DescriptorSetWeakReferenceObject> m_ds_wrefs;
     *  \brief DescriptorSets of all pipelines in target shader program. The order is same with pipelines in shader program.
     */
    std::vector<DescriptorSetWeakReferenceObject> m_ds_wrefs;
    bool m_is_linked;
};

______________SD_END_GRAPHICS_NAMESPACE______________