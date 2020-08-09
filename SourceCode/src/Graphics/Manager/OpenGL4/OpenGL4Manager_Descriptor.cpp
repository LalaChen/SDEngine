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

#if defined(_WIN32) || defined(_WIN64)

#include "LogManager.h"
#include "OpenGL4Manager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void OpenGL4Manager::CreateDescriptorSetLayout(DescriptorSetLayoutIdentity &io_identity,  const std::vector<UniformVariableDescriptorWeakReferenceObject> &i_uvd_wrefs)
{

}

void OpenGL4Manager::DestroyDescriptorSetLayout(DescriptorSetLayoutIdentity &io_identity)
{
}

void OpenGL4Manager::CreateDescriptorPool(DescriptorPoolIdentity &io_identity)
{
}

void OpenGL4Manager::DestroyDescriptorPool(DescriptorPoolIdentity &io_identity)
{
}

void OpenGL4Manager::AllocateDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool_wref, const DescriptorSetLayoutWeakReferenceObject &i_layout_wref)
{
}

void OpenGL4Manager::WriteUniformVariablesToDescriptorSet(const DescriptorSetIdentity &i_identity, const std::vector<UniformVariableWeakReferenceObject> &i_uv_wrefs)
{
}

void OpenGL4Manager::FreeDescriptorSet(DescriptorSetIdentity &io_identity, const DescriptorPoolWeakReferenceObject &i_pool_wref)
{
}

______________SD_END_GRAPHICS_NAMESPACE______________

#endif