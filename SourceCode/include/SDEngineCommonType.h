/* ==============  SD Engine License ==============
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

/*! \file      SDEngineCommonType.h
    \brief     Declare some basic type in engine.
    \author    Kuan-Chih, Chen
    \date      2019/02/02
    \copyright MIT License.
*/

#include <string>

//---------------------------- start of namespace SDEngine ----------------------------
namespace SDE
{
//---------------------------- start of namespace Basic ----------------------------
namespace Basic
{

/*! \typedef int CodePageID
	\brief CodePageID is the type for ID about text code. For example, CodePageID 936 is GBK2312, 950 is Big5, ...etc.
*/
typedef int CodePageID;

/*! \typedef std::string FilePathString
	\brief String of file path.
*/
typedef std::string FilePathString;

/*! \typedef char Byte;
	\brief represent byte.
*/
typedef char Byte;

/*! \typedef Byte* BytePtr;
	\brief represent byte array pointer.
*/
typedef Byte* BytePtr;

/*! \typedef unsigned char UByte;
	\brief represent byte.
*/
typedef unsigned char UByte;

/*! \typedef UByte* UBytePtr;
	\brief represent byte array pointer.
*/
typedef UByte* UBytePtr;
}
//---------------------------- end of namespace Basic ---------------------------

//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

/*! \typedef unsigned int GLCode; 
    \brief represent Open GL define value.
*/
typedef unsigned int GLCode;

/*! \typedef std::string UniformVarName;
	\brief Type of uniform variable name.
*/
typedef std::string UniformVarName;

/*! \typedef unsigned int VertexAttribLocation;
    \brief VA location.
*/
typedef unsigned int VertexAttribLocation;

/*! \typedef uint64_t BufferHandle;
    \brief Buffer object handle. In opengl, it means buffer object ID. In Vulkan, it means buffer handle.
*/
typedef uint64_t BufferHandle;

/*! \typedef unsigned int ShaderHandle;
    \brief Represent all hande of all kind of shader. In opengl, it means shader ID. In Vulkan, it means shader module handle.
*/
typedef uint64_t ShaderHandle;

/*! \typedef unsigned int ShaderProgramHandle;
	\brief Represent all ID of shader program. In opengl, it means shader ID. In Vulkan, it means shader module handle.
*/
typedef uint64_t ShaderProgramHandle;

/*! \typedef uint64_t FrameBufferHandle; In opengl, it means shader ID. In Vulkan, it means framebuffer handle.
	\brief Frame buffer object ID.
*/
typedef uint64_t FrameBufferHandle;

/*! \typedef uint64_t TextureHandle;
	\brief UI vertex index.
*/
typedef uint64_t TextureHandle;

/*! \typedef int UniformLocation;
	\brief Represent uniform location in shader.
*/
typedef int UniformLocation;

/*! \typedef int ActiveTextureID;
	\brief Represent active texture ID.
*/
typedef int ActiveTextureID;

/*! \typedef unsigned int ColorAttachmentID;
    \brief color attachment ID.
*/
typedef unsigned int ColorAttachmentID;

/*! \typedef unsigned short USVertexIndex;
	\brief US vertex index.
*/
typedef unsigned short USVertexIndex;

/*! \typedef unsigned short UIVertexIndex;
	\brief UI vertex index.
*/
typedef unsigned int UIVertexIndex;

//--- Image Related
/*! \typedef typedef unsigned char* ImageBufferAddr;
    \brief Like void*
*/
typedef unsigned char* ImageBufferAddr;

/*! \typedef size_t ImageBufferSize;
    \brief Memory buffer size
*/
typedef size_t ImageBufferSize;

/*! \typedef unsigned int Size_ui;
	\brief represent size.
*/
typedef unsigned int Size_ui;
//--- Layer ID.

/*! \typedef unsigned int LayerID;
	\brief represent LayerID.
*/
typedef unsigned int LayerID;

/*! \typedef unsigned int LayerMaskValue;
	\brief represent layer mask value.
*/
typedef unsigned int LayerMaskValue;

/*! \typedef unsigned int RenderOrder;
	\brief Type of render order.
*/
typedef unsigned int RenderOrder;

/*! \typedef unsigned int RenderOrderOffset;
	\brief Type of render order offset.
*/
typedef unsigned int RenderOrderOffset;

}
//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDEngine ----------------------------