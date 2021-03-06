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

/*! \file      Mesh.h
 *  \brief     Introduce of class Mesh.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/09/22
 *  \copyright MIT License.
 */
#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "VertexBufferUsage.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "CommandBuffer.h"
#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Mesh);

/*! \class Mesh
 *  \brief In our graphic system, Mesh is used to keep and operate all vertex attribute
 *         buffers in this class.
 */
class SDENGINE_CLASS Mesh : public Object
{
public:
    /*! \fn explicit Mesh(const ObjectName &i_object_name);
     *  \param i_object_name Name of this object.
     *  \brief Constructor of Mesh
     */
    explicit Mesh(const ObjectName &i_object_name);

    /*! \fn ~Mesh();
     *  \brief Destructor of Mesh.
     */
    virtual ~Mesh();
public:
    /*! \fn void RegisterVertexBuffer(VertexBufferUsageEnum i_usage, const VertexBufferStrongReferenceObject &i_va);
     *  \param [in] i_usage Register buffer to channels.
     *  \param [in] i_va Register target buffer. Please note that we will keep strong reference.
     *  \brief Register vertex buffer reference object to target channel. Please note that we will keep strong reference in
     *         mesh. If we want to update data, we need get weak reference at fitst.
     */
    void RegisterVertexBuffer(VertexBufferUsageEnum i_usage, const VertexBufferStrongReferenceObject &i_va);

    /*! \fn void RegisterIndexBuffer(const IndexBufferStrongReferenceObject &i_idx);
     *  \param [in] i_idx Register target buffer. Please note that we will keep strong reference.
     *  \brief Register index buffer reference object. Please note that we will keep strong reference in
     *         mesh. If we want to update data, we need get weak reference at fitst.
     */
    void RegisterIndexBuffer(const IndexBufferStrongReferenceObject &i_idx);

    /*! \fn void BindVertexBuffers(const CommandBufferWeakReferenceObject &i_cb);
     *  \param [in] i_cb Target recording buffer.
     *  \brief Bind vertex attributes for this command buffer.
     */
    void BindVertexBuffers(const CommandBufferWeakReferenceObject &i_cb);

    /*! \fn void BindIndexBuffer(const CommandBufferWeakReferenceObject &i_cb);
     *  \param [in] i_cb Target recording buffer.
     *  \brief Bind index buffer for this command buffer.
     */
    void BindIndexBuffer(const CommandBufferWeakReferenceObject &i_cb);

    /*! \fn void Render(const CommandBufferWeakReferenceObject &i_cb);
     *  \param [in] i_cb Target recording buffer.
     */
    void Render(const CommandBufferWeakReferenceObject &i_cb);
protected:
    /*! \var VertexBufferStrongReferenceObject m_vertex_attribs[VertexBufferUsage_MAX_DEFINE_VALUE];
     *  \brief Vertex attributes.
     */
    VertexBufferStrongReferenceObject m_vertex_attribs[VertexBufferUsage_MAX_DEFINE_VALUE];

    /*! \var IndexBufferStrongReferenceObject m_index_buffer;
     *  \brief Index buffer.
     */
    IndexBufferStrongReferenceObject m_index_buffer;
};

______________SD_END_GRAPHICS_NAMESPACE______________