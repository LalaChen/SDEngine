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

#include "LogManager.h"
#include "UniformVariable.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

UniformVariable::UniformVariable(const ObjectName &i_object_name)
: Object(i_object_name)
{
}

UniformVariable::~UniformVariable()
{
}

void UniformVariable::RegisterDescriptorSet(const DescriptorSetWeakReferenceObject& i_desc_set_wref)
{
    if (i_desc_set_wref.IsNull() == false) {
        return;
    }

    std::list<DescriptorSetWeakReferenceObject>::iterator iter;
    for (iter = m_target_set_wrefs.begin(); iter != m_target_set_wrefs.end(); ++iter) {
        if ((*iter).IsEqualTo(i_desc_set_wref) == true) {
            SDLOGE("Descriptor Set(%s) already registered.", i_desc_set_wref.GetConstRef().GetObjectName().c_str());
            return;
        }
    }
    m_target_set_wrefs.push_back(i_desc_set_wref);
}

______________SD_END_GRAPHICS_NAMESPACE______________
