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

/*! \file      AttachmentDescription.h
 *  \brief     Introduce of class AttachmentDescription.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/11/24
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "TextureFormat.h"
#include "SampleCount.h"
#include "AttachmentOperator.h"
#include "ImageLayout.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class AttachmentDescription
 *  \brief Class AttachmentDescription is used to specify attachment information.
 */
class SDENGINE_CLASS AttachmentDescription
{
public:
    /*! \fn AttachmentDescription();
     *  \brief Constructor of AttachmentDescription.
     */
    AttachmentDescription();

    /*! \fn ~AttachmentDescription();
     *  \brief Destructor of AttachmentDescription.
     */
    ~AttachmentDescription();
public:
    /*! \fn TextureFormatEnum m_format;
     *  \brief Tell us what format of buffer binded in this attachment should be.
     */
    TextureFormatEnum m_format;

    /*! \fn SampleCountEnum m_sample_counts;
     *  \brief Sample count.
     */
    SampleCountEnum m_sample_counts;

    /*! \fn AttachmentLoadOperatorEnum m_load_op;
     *  \brief Specify the action about read pixel value from buffer before entering subpass.
     *         Default is clear.(clear current buffer by clean values)
     */
    AttachmentLoadOperatorEnum m_load_op;

    /*! \fn AttachmentStoreOperatorEnum m_store_op;
     *  \brief Specify the action about writing pixel value to buffer after finishing subpass.
     *         Default is store.
     */
    AttachmentStoreOperatorEnum m_store_op;
    
    /*! \fn AttachmentLoadOperatorEnum m_stencil_load_op;
     *  \brief Specify the action about read stencil value from buffer before entering subpass.
     *         Default is clear.(clear current buffer by clean values)
     */
    AttachmentLoadOperatorEnum m_stencil_load_op;
    
    /*! \fn AttachmentStoreOperatorEnum m_stencil_store_op;
     *  \brief Specify the action about writing stencil value to buffer after finishing subpass.
     *         Default is store.
     */
    AttachmentStoreOperatorEnum m_stencil_store_op;
    
    /*! \fn ImageLayoutEnum m_initial_layout;
     *  \brief Description about buffer usage before entering subpass.
     */
    ImageLayoutEnum m_initial_layout;

    /*! \fn ImageLayoutEnum m_final_layout;
     *  \brief Description about buffer usage after finishing subpass.
     */
    ImageLayoutEnum m_final_layout;
};

______________SD_END_GRAPHICS_NAMESPACE______________