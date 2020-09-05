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

* /

/*! \file      FeatureApplication.h
 *  \brief     Introduce of class FeatureApplication.
 *  \author    Kuan-Chih, Chen
 *  \date      2020/09/01
 *  \copyright MIT License.
 */

#pragma once

#include <Application/Android/AndroidApplication.h>

#include "../../../../../Common/UnitTest_FeatureDeveloping/Code/Sample.h"

using SDE::App::AndroidApplication;

class FeatureApplication : public AndroidApplication
{
public:
    explicit FeatureApplication(const std::string &i_win_title, AAssetManager *i_asset_mgr, GraphicsLibraryEnum i_adopt_library, int i_argc, char **i_argv);

    ~FeatureApplication();
public:
    void Initialize() override;
    void InitializeGraphicsSystem() override;
    void ReleaseGraphicsSystem() override;
public:
    void Resize(CompHandle i_ns_handle, Size_ui32 i_w, Size_ui32 i_h) override;
    void Update() override;
public:
    void SetSampleIdx(uint32_t i_target_idx);
protected:
    std::vector<SampleStrongReferenceObject> m_sample_srefs;
    uint32_t m_cur_sample_idx;
};