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

#include "OpenXRAPIManager.h"

#include "LogManager.h"

using SDE::Basic::LogManager;

______________SD_START_OPENXR_NAMESPACE______________

SD_SINGLETON_DECLARATION_IMPL(OpenXRAPIManager);

OpenXRAPIManager::OpenXRAPIManager(const FilePathString &i_config_fp)
: m_XR_instance(XR_NULL_HANDLE)
, m_system_id(XR_NULL_SYSTEM_ID)
, m_form_factor(XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY)
, m_cur_frame_state{XR_TYPE_FRAME_STATE}
{
	SD_SINGLETON_DECLARATION_REGISTER;
	//
	m_basic_config.LoadFromFile(i_config_fp);
}

OpenXRAPIManager::~OpenXRAPIManager()
{
}

XrResult OpenXRAPIManager::Initialize(const std::string &i_win_title)
{
	if (m_XR_instance != XR_NULL_HANDLE) {
		SDLOGE("XrInstance already create!!!");
		return XR_ERROR_LIMIT_REACHED;
	}

	std::vector<const char*> exts;
	m_basic_config.CollectExtensionCstr(exts);
	//Create xrInstance.
	XrInstanceCreateInfo create_info{};
	create_info.type = XR_TYPE_INSTANCE_CREATE_INFO;
	create_info.enabledExtensionNames = exts.data();
	create_info.enabledExtensionCount = exts.size();
	create_info.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;
	strcpy_s(create_info.applicationInfo.applicationName, 127, i_win_title.c_str());
	strcpy_s(create_info.applicationInfo.engineName, 127, "SD Engine");

	XrResult result = xrCreateInstance(&create_info, &m_XR_instance);
	if (result != XR_SUCCESS) {
		SDLOGE("Create XrInstance failure(%x)!!!", result);
		throw std::runtime_error("Create XrInstance failure");
	}

	//Get SystemId
	XrSystemGetInfo sys_get_info{};
	sys_get_info.type = XR_TYPE_SYSTEM_GET_INFO;
	sys_get_info.formFactor = m_form_factor;
	result = xrGetSystem(m_XR_instance, &sys_get_info, &m_system_id);
	if (result != XR_SUCCESS) {
		SDLOGE("Get System ID failure(%x)!!!", result);
		throw std::runtime_error("Get System ID failure");
	}

	//Get System Props.
	m_system_props.type = XR_TYPE_SYSTEM_PROPERTIES;
	result = xrGetSystemProperties(m_XR_instance, m_system_id, &m_system_props);
	if (result != XR_SUCCESS) {
		SDLOGE("get system props failure(%x)!!!", result);
		throw std::runtime_error("get system props failure");
	}
	else {
		SDLOG("System Properties: Name=%s VendorId=%d",
			m_system_props.systemName,
			m_system_props.vendorId);
		SDLOG("System Graphics Properties: MaxWidth=%d MaxHeight=%d MaxLayers=%d",
			m_system_props.graphicsProperties.maxSwapchainImageWidth,
			m_system_props.graphicsProperties.maxSwapchainImageHeight,
			m_system_props.graphicsProperties.maxLayerCount);
		SDLOG("System Tracking Properties: OrientationTracking=%s PositionTracking=%s",
			m_system_props.trackingProperties.orientationTracking == XR_TRUE ? "True" : "False",
			m_system_props.trackingProperties.positionTracking == XR_TRUE ? "True" : "False");
	}

	uint32_t view_count = 0;
	result = xrEnumerateViewConfigurationViews(m_XR_instance, m_system_id,
		XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, 0, &view_count, nullptr);

	m_config_views.resize(view_count);
	result = xrEnumerateViewConfigurationViews(m_XR_instance, m_system_id,
		XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO,
		view_count, &view_count, m_config_views.data());

	for (uint32_t i = 0; i < m_config_views.size(); ++i) {
		SDLOG("Config View[%d]: Recommend(%d, %d, %d) Max Rect(%d, %d, %d)",
			i,
			m_config_views[i].recommendedImageRectWidth,
			m_config_views[i].recommendedImageRectHeight,
			m_config_views[i].recommendedSwapchainSampleCount,
			m_config_views[i].maxImageRectWidth,
			m_config_views[i].maxImageRectHeight,
			m_config_views[i].maxSwapchainSampleCount);
	}

	return result;
}

XrResult OpenXRAPIManager::InitializeSession(const XrSessionCreateInfo &i_info)
{
	XrSessionCreateInfo c_info = i_info;
	c_info.systemId = m_system_id;
	XrResult result = xrCreateSession(m_XR_instance, &c_info, &m_session);
	if (result != XR_SUCCESS) {
		SDLOGE("create session ID failure(%x)!!!", result);
	}
	return result;
}

Resolution OpenXRAPIManager::GetRecommendSize() const
{
	Resolution recommend_size;

	if (m_basic_config.m_wm_strategy == OpenXRWindowSize_RECOMMAND_SIZE) {
		recommend_size.SetResolution(m_config_views[0].recommendedImageRectWidth, m_config_views[0].recommendedImageRectHeight);
	}
	else if (m_basic_config.m_wm_strategy == OpenXRWindowSize_MAXIMUM_SIZE) {
		recommend_size.SetResolution(m_config_views[0].maxImageRectWidth, m_config_views[0].maxImageRectHeight);
	}
	else {
		recommend_size.SetResolution(m_basic_config.m_wm_width, m_basic_config.m_wm_height);
	}

	return recommend_size;
}

XrResult OpenXRAPIManager::CreateSwapchain(const XrSwapchainCreateInfo &i_info, XrSwapchain &io_swapchain)
{
	XrResult result = xrCreateSwapchain(m_session, &i_info, &io_swapchain);
	if (result != XR_SUCCESS) {
		SDLOGE("Create XrSwapchain failure(%d)", result);
	}
	return result;
}

XrResult OpenXRAPIManager::EnumerateSwapchainFormats(const XrSwapchain i_swapchain, std::vector<int64_t> &io_formats)
{
	uint32_t sw_format_count;
	XrResult result;
	result = xrEnumerateSwapchainFormats(m_session, 0, &sw_format_count, nullptr);
	if (result != XR_SUCCESS) {
		SDLOGE("Enumerate XrSwapchain format failure(%d)", result);
		return result;
	}
	
	io_formats.resize(sw_format_count);
	result = xrEnumerateSwapchainFormats(m_session, (uint32_t)io_formats.size(), &sw_format_count,
		io_formats.data());
	if (result != XR_SUCCESS) {
		SDLOGE("Get XrSwapchain format failure(%d)", result);
		return result;
	}

	return result;
}

XrResult OpenXRAPIManager::EnumerateSwapchainImages(const XrSwapchain i_swapchain, std::vector<XrSwapchainImageBaseHeader*> &io_images)
{
	uint32_t image_count = 0;
	XrResult result;
	result = xrEnumerateSwapchainImages(i_swapchain, 0, &image_count, nullptr);
	if (result != XR_SUCCESS) {
		SDLOGE("Enumerate XrSwapchain Images failure(%d)", result);
		return result;
	}
	io_images.resize(image_count);
	result = xrEnumerateSwapchainImages(i_swapchain,
		image_count, &image_count,
		reinterpret_cast<XrSwapchainImageBaseHeader*>(io_images.data()));
	if (result != XR_SUCCESS) {
		SDLOGE("Get XrSwapchain Images failure(%d)", result);
	}
	return result;
}

XrResult OpenXRAPIManager::GetReadyTextureOfSwapchain(const XrSwapchain i_swapchain, uint32_t &io_idx)
{
	XrSwapchainImageAcquireInfo acq_info = {};
	acq_info.type = XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO;
	XrResult result = XR_SUCCESS;
	result = xrAcquireSwapchainImage(i_swapchain, &acq_info, &io_idx);
	if (result != XR_SUCCESS) {
		SDLOGE("XR acquire image failure.");
	}
	return result;
}

XrResult OpenXRAPIManager::ReleaseSwapchainImage(const XrSwapchain i_swapchain)
{
	XrResult result;
	XrSwapchainImageReleaseInfo r_info = {};
	r_info.type = XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO;
	result = xrReleaseSwapchainImage(i_swapchain, &r_info);
	if (result != XR_SUCCESS) {
		SDLOGE("XR release image failure(%d).", result);
	}
	return result;
}

XrResult OpenXRAPIManager::DestroySwapchain(XrSwapchain &io_swapchain)
{
	XrResult result = xrDestroySwapchain(io_swapchain);
	if (result != XR_SUCCESS) {
		SDLOGE("Create XrSwapchain failure(%d)", result);
	}
	io_swapchain = XR_NULL_HANDLE;
	return result;
}

XrResult OpenXRAPIManager::WaitFrame(const XrFrameWaitInfo &i_info)
{
	XrResult result = xrWaitFrame(m_session, &i_info, &m_cur_frame_state);
	if (result != XR_SUCCESS) {
		SDLOGE("Wait Frame failure(%d)", result);
	}
	return result;
}

XrResult OpenXRAPIManager::BeginFrame(const XrFrameBeginInfo &i_info)
{
	XrResult result = xrBeginFrame(m_session, &i_info);
	if (result != XR_SUCCESS) {
		SDLOGE("Begin Frame failure(%d)", result);
	}
	return result;
}

XrResult OpenXRAPIManager::EndFrame(const XrFrameEndInfo &i_info)
{
	XrResult result = xrEndFrame(m_session, &i_info);
	if (result != XR_SUCCESS) {
		SDLOGE("End Frame failure(%d)", result);
	}
	return result;
}

XrResult OpenXRAPIManager::CreateSpace(const XrReferenceSpaceCreateInfo &i_info, XrSpace &io_space)
{
	XrResult result = xrCreateReferenceSpace(m_session, &i_info, &io_space);
	if (result != XR_SUCCESS) {
		SDLOGE("Create reference space failure(%d)", result);
	}
	return result;
}

XrResult OpenXRAPIManager::DestroySpace(XrSpace &io_space)
{
	XrResult result = xrDestroySpace(io_space);
	if (result != XR_SUCCESS) {
		SDLOGE("Destroy reference space failure(%d)", result);
	}
	return result;
}

XrResult OpenXRAPIManager::LocateViews(const XrViewLocateInfo &i_view_info, XrViewState &i_view_state, std::vector<XrView> &io_views)
{
	uint32_t view_c_output;
	XrResult result = xrLocateViews(m_session,
		&i_view_info, &i_view_state,
		io_views.size(), &view_c_output, io_views.data());
	if (result != XR_SUCCESS) {
		SDLOGE("Destroy reference space failure(%d)", result);
	}
	return result;
}

XrResult OpenXRAPIManager::Terminate()
{
	XrResult result;
	result = xrDestroySession(m_session);
	if (result != XR_SUCCESS) {
		SDLOGE("Destroy XrSession failure(%x)!!!", result);
	}
	m_session = XR_NULL_HANDLE;

	result = xrDestroyInstance(m_XR_instance);
	if (result != XR_SUCCESS) {
		SDLOGE("Destroy XrInstance failure(%x)!!!", result);
	}
	m_XR_instance = XR_NULL_HANDLE;
	return result;
}

void OpenXRAPIManager::ConvertXrViewToProjMatrix(const XrView &i_xr_view, float i_near, float i_far, Matrix4X4f &io_proj)
{
	const float tleft = tanf(i_xr_view.fov.angleLeft);
	const float tright = tanf(i_xr_view.fov.angleRight);

	const float tdown = tanf(i_xr_view.fov.angleDown);
	const float tup = tanf(i_xr_view.fov.angleUp);

	const float ta_width = tright - tleft;
	const float ta_height = tup - tdown;

	io_proj.m_matrix[0][0] = 2.0f / ta_width;
	io_proj.m_matrix[1][0] = 0.0f;
	io_proj.m_matrix[2][0] = (tleft + tright) / ta_width;
	io_proj.m_matrix[3][0] = 0.0f;

	io_proj.m_matrix[0][1] = 0.0f;
	io_proj.m_matrix[1][1] = 2.0f / ta_height;
	io_proj.m_matrix[2][1] = (tup + tdown) / ta_height;
	io_proj.m_matrix[3][1] = 0.0f;

	io_proj.m_matrix[0][2] = 0.0f;
	io_proj.m_matrix[1][2] = 0.0f;
	io_proj.m_matrix[2][2] = -(i_far) / (i_far - i_near);
	io_proj.m_matrix[3][2] = -(i_far * i_near) / (i_far - i_near);

	io_proj.m_matrix[0][3] = 0.0f;
	io_proj.m_matrix[1][3] = 0.0f;
	io_proj.m_matrix[2][3] = -1.0f;
	io_proj.m_matrix[3][3] = 0.0f;
}

void OpenXRAPIManager::CovertXrPoseToTransform(const XrPosef &i_pose, Transform &io_trans)
{
	io_trans.m_rotation = Quaternion(
		i_pose.orientation.x,
		i_pose.orientation.y,
		i_pose.orientation.z,
		i_pose.orientation.w);

	io_trans.m_position = Vector3f(
		i_pose.position.x,
		i_pose.position.y,
		i_pose.position.z,
		1.0f);
}

void OpenXRAPIManager::CovertXrPoseToWorldMatrix(const XrPosef &i_pose, Matrix4X4f &io_mat)
{
	Transform target_trans;
	CovertXrPoseToTransform(i_pose, target_trans);
	io_mat = target_trans.MakeWorldMatrix();
}

_______________SD_END_OPENXR_NAMESPACE_______________

