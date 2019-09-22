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
#include "Object.h"

using SDE::Basic::ObjectName;
using SDE::Basic::Object;

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(Mesh);

/*! \class Mesh
 *  In our graphic system, Mesh is used to keep and operate all vertex attribute
 *  buffers in this class.
 */

class SDENGINE_CLASS Mesh : public Object
{
public:
    explicit Mesh(const ObjectName &i_object_name);
    virtual ~Mesh();
protected:
    VertexBufferStrongReferenceObject m_vertex_attribs[VertexBufferUsage_MAX_DEFINE_VALUE];
};

______________SD_END_GRAPHICS_NAMESPACE______________