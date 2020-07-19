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

/*! \file      UniformImages.h
 *  \brief     Introduce of class about UniformImages.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/06/27
 *  \copyright MIT License.
 */

#pragma once

#include "Texture.h"
#include "UniformImagesDescriptor.h"
#include "UniformVariable.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

SD_DECLARE_STRONG_AMD_WEAK_REF_TYPE(UniformImages);

class UniformImages : public UniformVariable
{
public:
    /*! \fn explicit UniformImages(const ObjectName &i_object_name, uint32_t i_binding_id);
     *  \param [in] i_object_name Name of this object.
     *  \param [in] i_binding_id ID of shader binding.
     *  \brief Constructor of UniformImages.
     */
    explicit UniformImages(const ObjectName &i_object_name, uint32_t i_binding_id);

    /*! \fn virtual ~UniformImages();
     *  \brief Destructor of UniformImages.
     */
    virtual ~UniformImages();

public:
    void Initialize(const UniformImagesDescriptorWeakReferenceObject &i_uid_wref);

public:
    inline UniformBindingTypeEnum GetType() const override;

public:
    Size_ui32 GetAmount() const;

    const std::vector<TextureWeakReferenceObject>& GetTextures() const;

protected:
    std::vector<TextureWeakReferenceObject> m_tex_wrefs;
};

inline UniformBindingTypeEnum UniformImages::GetType() const
{
    return UniformBindingType_COMBINED_IMAGE_SAMPLER;
}

inline Size_ui32 UniformImages::GetAmount() const
{
    return static_cast<Size_ui32>(m_tex_wrefs.size());
}

inline const std::vector<TextureWeakReferenceObject>& UniformImages::GetTextures() const
{
    return m_tex_wrefs;
}

______________SD_END_GRAPHICS_NAMESPACE______________