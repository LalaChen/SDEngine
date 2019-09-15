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
 *  \brief     Declare some basic type in engine.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/02
 *  \copyright MIT License.
 */
#include <string>

#include "SDEngineMacro.h"

______________SD_START_BASIC_NAMESPACE_______________

/*! \typedef int CodePageID
 * \brief CodePageID is the type for ID about text code. For example, CodePageID 936 is GBK2312, 950 is Big5, ...etc.
 */
typedef int CodePageID;

/*! \typedef std::string FilePathString
 *  \brief String of file path.
 */
typedef std::string FilePathString;

/*! \typedef char Byte;
 *  \brief represent byte.
 */
typedef char Byte;

/*! \typedef Byte* BytePtr;
 *  \brief represent byte array pointer.
 */
typedef Byte* BytePtr;

/*! \typedef unsigned char UByte;
    \brief represent byte.
*/
typedef unsigned char UByte;

/*! \typedef UByte* UBytePtr;
 *  \brief represent byte array pointer.
 */
typedef UByte* UBytePtr;

/*! \typedef std::string ObjectName;
 *  \brief name of object.
 */
typedef std::string ObjectName;

_______________SD_END_BASIC_NAMESPACE________________

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \typedef void* VoidPtr;
 *  \brief Void handle.
 */
typedef void* VoidPtr;

/*! \typedef unsigned int APICode; 
 *  \brief represent API define value or enum.
 */
typedef unsigned int APICode;

/*! \typedef std::string UniformVarName;
 *  \brief Type of uniform variable name.
 */
typedef std::string UniformVarName;

/*! \typedef uint32_t VertexAttribLocation;
 *  \brief VA location.
 */
typedef uint32_t VertexAttribLocation;

/*! \typedef uint64_t CompHandle;
 *  \brief graphics component(Buffer, Texture, ...etc) handle. In opengl, it means object ID. In Vulkan, it means handle.
 */
typedef uint64_t CompHandle;

/*! \typedef int32_t UniformLocation;
 *  \brief Represent uniform location in shader.
 */
typedef int32_t UniformLocation;

/*! \typedef int32_t ActiveTextureID;
 *	\brief Represent active texture ID.
 */
typedef int32_t ActiveTextureID;

/*! \typedef uint32_t ColorAttachmentID;
 *  \brief color attachment ID.
 */
typedef uint32_t ColorAttachmentID;

/*! \typedef uint16_t USVertexIndex;
 *	\brief US vertex index.
 */
typedef uint16_t USVertexIndex;

/*! \typedef uint32_t UIVertexIndex;
 *  \brief UI vertex index.
 */
typedef uint32_t UIVertexIndex;

//--- Image Related
/*! \typedef typedef unsigned char* ImageBufferAddr;
 *  \brief Like void*
 */
typedef unsigned char* ImageBufferAddr;

/*! \typedef size_t ImageBufferSize;
 *  \brief Memory buffer size
 */
typedef size_t ImageBufferSize;

/*! \typedef uint32_t Size_ui32;
 *  \brief represent size.
 */
typedef uint32_t Size_ui32;

/*! \typedef uint64_t Size_ui64;
 *  \brief represent size.
 */
typedef uint64_t Size_ui64;
//--- Layer ID.

/*! \typedef uint32_t LayerID;
 *  \brief represent LayerID.
 */
typedef uint32_t LayerID;

/*! \typedef uint32_t LayerMaskValue;
 *  \brief represent layer mask value.
 */
typedef uint32_t LayerMaskValue;

/*! \typedef uint32_t RenderOrder;
 *  \brief Type of render order.
 */
typedef uint32_t RenderOrder;

/*! \typedef uint32_t RenderOrderOffset;
 *  \brief Type of render order offset.
 */
typedef uint32_t RenderOrderOffset;

______________SD_END_GRAPHICS_NAMESPACE______________