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

/*! \file      SamplerIdentity.h
 *  \brief     Introduce of class SamplerIdentity.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/08/12
 *  \copyright MIT License.
 */

#pragma once

#include "GraphicsElementIdentity.h"
#include "ManagerParam.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class SamplerIdentity
 *  \brief Keep all sampling parameters (mag and min filter, texture wrap mode and etc ...)in this struct.
 */
class SDENGINE_CLASS SamplerIdentity : public GraphicsElementIdentity
{
public:
    /*! \fn explicit SamplerIdentity();
     *  \brief The constructor of SamplerIdentity Class.
     */
    SamplerIdentity();

    /*! \fn explicit ~SamplerIdentity();
     *  \brief The destructor of SamplerIdentity Class.
     */
    ~SamplerIdentity();
public://Sampler Part.
    /*! \var CompHandle m_handle;
     *  \brief Tell us how to sample this texture.
     */
    CompHandle m_handle;

    /*! \var SamplerFilterTypeEnum m_mag_filter_type;
     *  \brief The mag filter type. Default is nearest.
     */
    SamplerFilterTypeEnum m_mag_filter_type;

    /*! \var SamplerFilterTypeEnum m_min_filter_type;
     *  \brief The min filter type. Default is nearest.
     */
    SamplerFilterTypeEnum m_min_filter_type;

    /*! \var SamplerMipmapModeEnum m_mipmap_mode;
     *  \brief The sampler mipmap mode. Default is nearest.
     */
    SamplerMipmapModeEnum m_mipmap_mode;

    /*! \var SamplerWrapModeEnum m_wrap_mode_s;
     *  \brief The texture wrap type about coordinate s axis.
     */
    SamplerWrapModeEnum m_wrap_mode_s;

    /*! \var SamplerWrapModeEnum m_wrap_mode_t;
     *  \brief The texture wrap type about coordinate t axis.
     */
    SamplerWrapModeEnum m_wrap_mode_t;

    /*! \var SamplerWrapModeEnum m_wrap_mode_r;
     *  \brief The texture wrap type about coordinate r(z) axis.
     */
    SamplerWrapModeEnum m_wrap_mode_r;

    /*! \var SamplerBorderColorTypeEnum m_sampler_b_color_type;
     *  \brief The sampler border color type.
     */
    SamplerBorderColorTypeEnum m_sampler_b_color_type;

    /*! \var bool m_use_compare;
     *  \brief To decide do we enable compare function.
     *        (Usual use at comparing depth with SamplerShadow2D).
     */
    bool m_use_compare;

    /*! \var CompareOperatorEnum m_compare_op;
     *  \brief The compared enable.
     */
    CompareOperatorEnum m_compare_op;

    /*! \var bool m_use_anisotropy;
     *  \brief To enable use anisotropy when we calcuate level of detail.
     */
    bool m_use_anisotropy;

    /*! \var float m_max_anisotropy;
     *  \brief To define max anisotropy.
     */
    float m_max_anisotropy;
};

______________SD_END_GRAPHICS_NAMESPACE______________