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

/*! \file      OpenXRAPIManager.h
 *  \brief     Introduce of class OpenXRAPIManager
 *  \author    Kuan-Chih, Chen
 *  \date      2022/06/05
 *  \copyright MIT License.
 */



#pragma once

#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

#include "SDEngineMacro.h"
#include "Transform.h"
#include "Resolution.h"
#include "OpenXRConfig.h"

using SDE::Math::Transform;
using SDE::Math::Quaternion;
using SDE::Math::Matrix4X4f;
using SDE::Math::Vector3f;
using SDE::Basic::FilePathString;
using SDE::Graphics::CompHandle;
using SDE::Graphics::Resolution;

______________SD_START_OPENXR_NAMESPACE______________

class SDENGINE_CLASS OpenXRAPIManager
{
public:
	SD_SINGLETON_DECLARATION(OpenXRAPIManager);
public:
	explicit OpenXRAPIManager(const FilePathString &i_config_gp);
	virtual ~OpenXRAPIManager();
public:
	XrResult Initialize(const std::string &i_win_title);
	XrResult InitializeSession(const XrSessionCreateInfo &i_info);
	XrResult CreateSwapchain(const XrSwapchainCreateInfo &i_info, XrSwapchain &io_swapchain);
	XrResult DestroySwapchain(XrSwapchain &io_swapchain);
	XrResult EnumerateSwapchainImages(const XrSwapchain i_swapchain, std::vector<XrSwapchainImageBaseHeader*> &io_images);
	XrResult GetReadyTextureOfSwapchain(const XrSwapchain i_swapchain, uint32_t &io_idx);
	XrResult ReleaseSwapchainImage(const XrSwapchain i_swapchain);
	XrResult EnumerateSwapchainFormats(const XrSwapchain i_swapchain, std::vector<int64_t> &io_formats);
	XrResult WaitFrame(const XrFrameWaitInfo &i_info);
	XrResult BeginFrame(const XrFrameBeginInfo &i_info);
	XrResult EndFrame(const XrFrameEndInfo &i_info);
	XrResult CreateSpace(const XrReferenceSpaceCreateInfo &i_info, XrSpace &io_space);
	XrResult DestroySpace(XrSpace &io_space);
	XrResult LocateViews(const XrViewLocateInfo &i_view_info, XrViewState &i_view_state, std::vector<XrView> &io_views);
	XrResult Terminate();
public:
	void ConvertXrViewToProjMatrix(const XrView &i_xr_view, float i_near, float i_far, Matrix4X4f &io_proj);
	void CovertXrPoseToTransform(const XrPosef &i_pose, Transform &io_trans);
	void CovertXrPoseToWorldMatrix(const XrPosef &i_pose, Matrix4X4f &io_mat);
public:
	bool ShouldRendering() const;
	XrTime GetPredictedDisplayTime() const;
	XrDuration GetPredictedDisplayPeriod() const;
	Resolution GetRecommendSize() const;
	XrEnvironmentBlendMode GetBlendMode() const;
protected:
	XrInstance   m_XR_instance;
	XrSystemId   m_system_id;
	XrFormFactor m_form_factor;
	XrSession    m_session;
protected:
	XrFrameState m_cur_frame_state;
protected:
	XrSystemProperties m_system_props;
	std::vector<XrViewConfigurationView> m_config_views;
	XrEnvironmentBlendMode m_env_blending_mode;
protected:
	OpenXRConfig m_basic_config;
};

inline XrEnvironmentBlendMode OpenXRAPIManager::GetBlendMode() const
{
	return m_env_blending_mode;
}

inline bool OpenXRAPIManager::ShouldRendering() const
{
	return m_cur_frame_state.shouldRender;
}

inline XrTime OpenXRAPIManager::GetPredictedDisplayTime() const
{
	return m_cur_frame_state.predictedDisplayTime;
}

inline XrDuration OpenXRAPIManager::GetPredictedDisplayPeriod() const
{
	return m_cur_frame_state.predictedDisplayPeriod;
}
_______________SD_END_OPENXR_NAMESPACE_______________