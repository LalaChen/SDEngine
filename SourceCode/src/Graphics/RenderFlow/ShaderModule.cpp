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
#include "FileSystemManager.h"
#include "GraphicsManager.h"
#include "ShaderModule.h"

using SDE::Basic::FileData;
using SDE::Basic::FileSystemManager;

_____________SD_START_GRAPHICS_NAMESPACE_____________

ShaderModule::ShaderModule(const ObjectName i_shader_name)
: Object(i_shader_name)
{
}

ShaderModule::~ShaderModule()
{
    if (m_identity.m_prog_handle != SD_NULL_HANDLE) {
        GraphicsManager::GetRef().DeleteShaderModule(m_identity);
    }
}

bool ShaderModule::LoadBinaryShader(ShaderKindEnum i_kind, const FilePathString &i_binary_fp, const char *i_entry_name)
{
    FileData fd;
    FileSystemManager::GetRef().OpenFile(i_binary_fp, fd);
    if (fd.IsEmpty() == false) {
        return (LoadBinaryShader(i_kind, fd.m_file_content, i_entry_name) == 1);
    }
    else {
        SDLOGW("File [%s] can't be open!!!", i_binary_fp.c_str());
        return false;
    }
}

int32_t ShaderModule::LoadBinaryShader(ShaderKindEnum i_kind, const std::vector<UByte> &i_content, const char *i_entry_name)
{
    if (i_content.size() > 0) {

        GraphicsManager::GetRef().CreateShaderModule(m_identity, i_content);

        if (m_identity.m_prog_handle != SD_NULL_HANDLE) {
            m_identity.m_shader_kind = i_kind;
            m_identity.m_entry_name = i_entry_name;
            return 1;
        }
        else {
            SDLOGW("We can't load binary from this content!!!");
            return -1;
        }
    }
    else {
        SDLOGW("Content is empty!!!");
        return -2;
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________