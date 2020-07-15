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

#include "UniformImages.h"
#include "UniformImagesDescriptor.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

UniformImagesDescriptor::UniformImagesDescriptor(const ObjectName &i_name, Size_ui32 i_number)
: UniformVariableDescriptor(i_name, i_number)
{
}

UniformImagesDescriptor::~UniformImagesDescriptor()
{
}

UniformVariableStrongReferenceObject UniformImagesDescriptor::AllocateUniformVariable()
{
    UniformImagesStrongReferenceObject ub_sref = new UniformImages(m_object_name);
    UniformImagesDescriptorWeakReferenceObject this_wref = GetThisWeakPtrByType<UniformImagesDescriptor>();
    ub_sref.GetRef().Initialize(this_wref);
    return ub_sref.StaticCastTo<UniformVariable>();
}

______________SD_END_GRAPHICS_NAMESPACE______________