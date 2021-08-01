#pragma once

#include <wvr/wvr.h>
#include <wvr/wvr_render.h>
#include <wvr/wvr_device.h>
#include <wvr/wvr_projection.h>
#include <wvr/wvr_overlay.h>
#include <wvr/wvr_system.h>
#include <wvr/wvr_events.h>
#include <wvr/wvr_vulkan.h>

#include <SDEngineMacro.h>
#include <SDEngineCommonType.h>
#include <SDEngine.h>

using namespace SDE;
using namespace SDE::Math;
using namespace SDE::Basic;
using namespace SDE::Graphics;

static Matrix4X4f ConvertWVR_Matrix4f_tToMatrix4X4f(const WVR_Matrix4f_t &i_wvr_mat)
{
    return Matrix4X4f(glm::mat4(
            i_wvr_mat.m[0][0], i_wvr_mat.m[1][0], i_wvr_mat.m[2][0], i_wvr_mat.m[3][0],
            i_wvr_mat.m[0][1], i_wvr_mat.m[1][1], i_wvr_mat.m[2][1], i_wvr_mat.m[3][1],
            i_wvr_mat.m[0][2], i_wvr_mat.m[1][2], i_wvr_mat.m[2][2], i_wvr_mat.m[3][2],
            i_wvr_mat.m[0][3], i_wvr_mat.m[1][3], i_wvr_mat.m[2][3], i_wvr_mat.m[3][3]));
}

static Transform ConvertWVR_PoseState_tToMatrix4X4f(const WVR_PoseState_t &i_wvr_pose)
{
    Matrix4X4f mat = ConvertWVR_Matrix4f_tToMatrix4X4f(i_wvr_pose.poseMatrix);
    return Transform(mat);
}