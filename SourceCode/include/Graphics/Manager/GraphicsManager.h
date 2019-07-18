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

/*! \file      GraphicsManager.h
 *  \brief     Introduce of class GraphicsManager
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/03
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"
#include "ManagerParam.h"
#include "ManagerIdentity.h"
#include "Resolution.h"
#include "EventArg.h"

using SDE::Basic::EventArg;

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

enum GraphicsLibraryEnum {
    GraphicsLibrary_OpenGL4 = 0,
    GraphicsLibrary_Vulkan  = 1,
    GraphicsLibrary_MaxEnum,
    GraphicsLibrary_Unknown = GraphicsLibrary_MaxEnum
};

/*! \class GraphicsManager
 *  In our system, GraphicsManager is a interface for all graphics API(opengl, gles, vulkan) and manage \n
 *  graphics resource those need to be managed.
 */
class SDENGINE_CLASS GraphicsManager
{
public:
    SD_SINGLETON_DECLARATION(GraphicsManager);
public:
    /*! \fn GraphicsManager();
     *  \brief The constructor of GraphicsManager Class. 
     */
    GraphicsManager();
    
    /*! \fn virtual ~GraphicsManager();
     *  \brief The destructor of GraphicsManager Class.
     */
    virtual ~GraphicsManager();
public:
//------------ Initialize and Release -------------
    /*! \fn virtual void InitializeGraphicsSystem(const EventArg &i_arg) = 0;
     *  \param [in] i_arg argumnet for initializing manager. We should define necessary arguments class(derived EventArg) and input those arguments.
     *  \brief Initialize graphics system. (link dll, ...)
     */
    virtual void InitializeGraphicsSystem(const EventArg &i_arg) = 0;
    
    /*! \fn virtual void ReleaseGraphicsSystem() = 0;
     *  \brief Release graphics system.
     */
    virtual void ReleaseGraphicsSystem() = 0;
public:
    /*! \fn virtual void PrintSystemInformation() = 0;
     *  \brief Print system information.
     */
    virtual void PrintSystemInformation() = 0;
public:
//----------- Vertex Buffer Function ------------
    virtual void CreateVertexBuffer(VertexBufferIdentity &io_identity, Size_ui64 i_data_size, VertexBufferMemoryTypeEnum i_memory_type) = 0;
    virtual void RefreshStaticVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) = 0;
    virtual void RefreshDynamicVertexBuffer(const VertexBufferIdentity &i_identity, void *i_data_ptr, Size_ui64 i_data_size) = 0;
    virtual void DeleteVertexBuffer(VertexBufferIdentity &io_identity) = 0;
    virtual void MapBuffer(const VertexBufferIdentity &i_identity, VoidHandle &io_buffer_handle) = 0;
    virtual void UnmapBuffer(const VertexBufferIdentity &i_identity) = 0;
public:
//------------- Resize Function -----------------
    virtual void Resize(int i_w, int i_h) = 0;
public:
//------------- Render Function -----------------
    void Render();
protected:
//------------ Render Flow Function -------------
    virtual void RenderBegin() = 0;
    virtual void RenderToScreen() = 0;
    virtual void RenderEnd() = 0;
protected:
    SD_DECLARE_ATTRIBUTE_VAR_GET(Resolution, m_screen_size, ScreenResolution);
};

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}