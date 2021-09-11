/*==============  SD Engine License ==============
MIT License

Copyright(c) 2019 Kuan - Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/*! \file      AndroidEnvAttacher.h
 *  \brief     Introduce of class AndroidEnvAttacher.
 *  \author    Kuan-Chih, Chen
 *  \date      2021/09/11
 *  \copyright MIT License.
 */

#include "LogManager.h"
#include "AndroidEnvAttacher.h"

using SDE::Basic::LogManager;

________________SD_START_APP_NAMESPACE_______________

AndroidEnvAttacher::AndroidEnvAttacher(JavaVM *i_vm)
:m_env(nullptr)
,m_target_VM(i_vm)
,m_is_attach_thread(false)
{
    int env_result = m_target_VM->GetEnv((void**)&m_env, JNI_VERSION_1_6);
    if (env_result == JNI_EDETACHED) {
        SDLOG("GetEnv : JNI_EDETACHED");
        if (m_target_VM->AttachCurrentThread(&m_env , nullptr) != 0) {
            SDLOG("GetEnv : Failed to attach");
        }
        m_is_attach_thread = true;
    }
    else if (env_result == JNI_EVERSION) {
        SDLOG("GetEnv: version not supported");
    }

}

AndroidEnvAttacher::~AndroidEnvAttacher()
{
    if(m_is_attach_thread == true)
         m_target_VM->DetachCurrentThread();
    m_target_VM = nullptr;
    m_env = nullptr;
}

JNIEnv* AndroidEnvAttacher::GetEnv()
{
    return m_env;
}

_________________SD_END_APP_NAMESPACE________________
