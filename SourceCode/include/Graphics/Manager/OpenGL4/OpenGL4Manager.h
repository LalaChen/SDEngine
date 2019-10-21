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

/*! \file      OpenGL4Manager.h
 *  \brief     Introduce of class OpenGL4Manager
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/03
 *  \copyright MIT License.
 */
#pragma once

#include "GraphicsManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class OpenGL4Manager
 *  In our system, OpenGL4Manager is a implementation for opengl4 graphics API.
 */
class SDENGINE_CLASS OpenGL4Manager : public GraphicsManager
{
public:
    /*! \fn OpenGL4Manager();
     *  \brief The constructor of OpenGL4Manager Class.
     */
    OpenGL4Manager();
    
    /*! \fn virtual ~OpenGL4Manager();
     *  \brief The destructor of OpenGL4Manager Class.
     */
    virtual ~OpenGL4Manager();
public:
    /*! \fn void InitializeGraphicsSystem() override;
     *  \param [in] i_arg Nothing. implement for following interface.
     *  \brief Initialize graphics API. (link dll, ...)
     */
    void InitializeGraphicsSystem(const EventArg &i_arg) override;
    
    /*! \fn void ReleaseGraphicsSystem() override;
     *  \brief Release graphics API.
     */
    void ReleaseGraphicsSystem() override;
public:
    /*! \fn void PrintSystemInformation() override;
     *  \brief Print system information.
     */
    void PrintSystemInformation() override;
public:
//----------- Vertex Buffer Function ------------
    void CreateVertexBuffer(VertexBufferIdentity &io_identity, Size_ui64 i_data_size, VertexBufferMemoryTypeEnum i_memory_type) override;
    void RefreshStaticVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) override;
    void RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) override;
    void DeleteVertexBuffer(VertexBufferIdentity &io_identity) override;
    void MapBuffer(const VertexBufferIdentity &i_identity, VoidPtr &io_buffer_handle) override;
    void UnmapBuffer(const VertexBufferIdentity &i_identity) override;
public:
    void CreateTextureImage(TextureIdentity &io_identity, SamplerIdentity &io_sampler_identity, VoidPtr i_data_ptr, Size_ui64 i_data_size) override;
    void RefreshTextureImage(const TextureIdentity &i_identity, VoidPtr i_data_ptr, ImageOffset i_offset, ImageSize i_size, Size_ui64 i_data_size) override;
    void DeleteTextureImage(TextureIdentity &io_identity) override;
public:
    void Resize(Size_ui32 i_w, Size_ui32 i_h) override;
protected:
//--------------- Render Flow Function ------------------
    void RenderBegin() override;
    void RenderToScreen() override;
    void RenderEnd() override;
};

______________SD_END_GRAPHICS_NAMESPACE______________