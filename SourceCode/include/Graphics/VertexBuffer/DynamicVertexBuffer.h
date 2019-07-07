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

/*! \file      DynamicVertexBuffer.h
 *  \brief     Introduce of class DynamicVertexBuffer.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/07/03
 *  \copyright MIT License.
 */

#pragma once

#include "VertexBuffer.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

/*! \class StaticVertexBuffer
 *  In our graphic system, StaticVertexBuffer objects are used to keep vertex datas.
 */
SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(DynamicVertexBuffer);

class SDENGINE_API DynamicVertexBuffer : public VertexBuffer
{
public:
    /*! \fn explicit DynamicVertexBuffer(const ObjectName &i_object_name, VertexAttribLocation i_va_location, VBufferFormatEnum i_format);
     *  \param [in] i_object_name Name of this buffer.
     *  \param [in] i_va_location Attribute location of this buffer.
     *  \param [in] i_value_type The value type of this buffer.
     *  \brief The constructor of DynamicVertexBuffer Class.
     */
    explicit DynamicVertexBuffer(const ObjectName &i_object_name, VertexAttribLocation i_va_location, VBufferFormatEnum i_format);

    /*! \fn ~DynamicVertexBuffer()
     *  \brief The destructor of DynamicVertexBuffer Class.
     */
    virtual ~DynamicVertexBuffer();
public:
    /* \fn void RefreshBufferData(void *i_data_ptr, Size_ui64 i_data_size) override;
     * \param [in] i_data_ptr Data pointer.
     * \param [in] i_data_size Data size.
     * \brief Refresh data into buffer.
     */
    void RefreshBufferData(void *i_data_ptr, Size_ui64 i_data_size) override;
};

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}