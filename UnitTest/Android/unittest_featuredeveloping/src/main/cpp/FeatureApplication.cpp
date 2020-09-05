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

#include <SDEngine.h>
#include <SDEnginePlatform.h>

#include "../../../../../Common/UnitTest_FeatureDeveloping/Code/SampleDrawObjects.h"
#include "FeatureApplication.h"

using namespace SDE;
using namespace SDE::Basic;
using namespace SDE::Graphics;

FeatureApplication::FeatureApplication(const std::string &i_win_title, AAssetManager *i_asset_mgr, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv)
: AndroidApplication(i_win_title, i_asset_mgr, i_adopt_library, i_argc, i_argv)
, m_cur_sample_idx(0)
{
}

FeatureApplication::~FeatureApplication()
{
}


void FeatureApplication::Initialize()
{
    AndroidApplication::Initialize();
    //Create Sample
    m_sample_srefs.push_back(new SampleDrawObjects("SampleDrawObject"));
}

void FeatureApplication::InitializeGraphicsSystem()
{
    AndroidApplication::InitializeGraphicsSystem();

    std::vector<SampleStrongReferenceObject>::iterator iter;
    for (iter = m_sample_srefs.begin(); iter != m_sample_srefs.end(); ++iter) {
        (*iter).GetRef().Initialize();
    }
}

void FeatureApplication::Update()
{
    // Override Update for temporary.
    Timer::GetRef().Update();
    //
    if (m_sample_srefs[m_cur_sample_idx].IsNull() == false) {
        m_sample_srefs[m_cur_sample_idx].GetRef().Render();
        //
        GraphicsManager::GetRef().RenderTexture2DToScreen(m_sample_srefs[m_cur_sample_idx].GetRef().GetColorBuffer());
    }
}

void FeatureApplication::Resize(CompHandle i_ns_handle, Size_ui32 i_w, Size_ui32 i_h)
{
    GraphicsManager::GetRef().Resize(i_ns_handle, i_w, i_h);

    for (uint32_t sample_idx = 0; sample_idx < m_sample_srefs.size(); ++sample_idx) {
        m_sample_srefs[sample_idx]->Resize(i_w, i_h);
    }
}

void FeatureApplication::ReleaseGraphicsSystem()
{
    //
    for (SampleStrongReferenceObject &sample_sref : m_sample_srefs) {
        if (sample_sref != nullptr) {
            sample_sref.GetRef().Destroy();
            sample_sref.Reset();
        }
    }
    m_sample_srefs.resize(0);
    m_sample_srefs.shrink_to_fit();

    AndroidApplication::ReleaseGraphicsSystem();
}

void FeatureApplication::SetSampleIdx(uint32_t i_target_idx)
{
    if (i_target_idx < m_sample_srefs.size()) {
        m_cur_sample_idx = i_target_idx;
    }
}