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

/*! \file      AttachmentReference.h
 *  \brief     Introduce of class AttachmentReference.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/11/24
 *  \copyright MIT License.
 */
#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "ImageLayout.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class AttachmentReference
 *  \brief Class AttachmentReference is used to specify attachment type.
 */
class SDENGINE_CLASS AttachmentReference
{
public:
    /*! \fn AttachmentReference();
     *  \brief Constructor of AttachmentReference.
     */
    AttachmentReference();

    /*! \fn ~AttachmentReference();
     *  \brief Destructor of AttachmentReference.
     */
    ~AttachmentReference();

public:
    /*! \var uint32_t m_attachment_ID;
     *  \brief Specify this attachment refers to which attachment in this render pass.
     */
    uint32_t m_attachment_ID;

    /*! \var ImageLayoutEnum m_layout;
     *  \brief Specify the usage about this attachment.
     */
    ImageLayoutEnum m_layout;
};
______________SD_END_GRAPHICS_NAMESPACE______________