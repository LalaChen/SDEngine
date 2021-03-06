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

/*! \file      VertexAttribBinding.h
 *  \brief     Introduce of classes about vertex attribution.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/10/26
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "VertexBufferFormat.h"
#include "VertexInputRate.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class VertexAttribBindingDescription
 *  VertexAttribBindingDescription is used to describe the usage and format about buffer
 *  which bind in this channel. We will use this class to descibe for every channel
 *  in GraphicsPipeline.
 */
class SDENGINE_CLASS VertexAttribBindingDescription
{
public:
    /*! \fn VertexAttribBindingDescription();
     *  \brief Constructor of VertexAttribBindingDescription.
     */
    VertexAttribBindingDescription();

    /*! \fn ~VertexAttribBindingDescription();
     *  \brief Destructor of VertexAttribBindingDescription.
     */
    ~VertexAttribBindingDescription();
public:
    /*! \var uint32_t m_binding_id;
     *  \brief Buffer binding channel ID.
     */
    uint32_t m_binding_id;

    /*! \var uint32_t m_binding_id;
     *  \brief Bytes of one element for this buffer.
     */
    uint32_t m_stride;

    /*! \var uint32_t m_binding_id;
     *  \brief Using for vertex or instance.
     */
    VertexInputRateEnum m_input_rate;
};


/*! \class VertexAttribLocationDescription
 *  VertexAttribLocationDescription is used to link buffer binding channel and 
 *  attribute location. We will use this class to descibe the relationship between
 *  all binding and location of vertex shader in GraphicsPipeline.
 */
class SDENGINE_CLASS VertexAttribLocationDescription
{
public:
    /*! \fn VertexAttribLocationDescription();
     *  \brief Constructor of VertexAttribLocationDescription.
     */
    VertexAttribLocationDescription();

    /*! \fn ~VertexAttribBindingDescription();
     *  \brief Destructor of VertexAttribLocationDescription.
     */
    ~VertexAttribLocationDescription();
public:
    /*! \var uint32_t m_binding_id;
     *  \brief Buffer binding channel ID.
     */
    uint32_t m_binding_id;

    /*! \var uint32_t m_location;
     *  \brief Target vertex attribute location.
     */
    uint32_t m_location;

    /*! \var VertexBufferFormatEnum m_format;
     *  \brief Data format about this vertex attribute in this location.
     */
    VertexBufferFormatEnum m_format;

    /*! \var uint32_t m_offset;
     *  \brief Offset when we read data in the binding for this vertex attribute.
     */
    uint32_t m_offset;
};

______________SD_END_GRAPHICS_NAMESPACE______________