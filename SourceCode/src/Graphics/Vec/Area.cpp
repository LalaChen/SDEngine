/*
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

#include "Area.h"
#include "LogManager.h"

using SDE::Basic::LogManager;

_____________SD_START_GRAPHICS_NAMESPACE_____________

Area2D::Area2D()
: x(0.0f), y(0.0f), w(0.0f), h(0.0f)
{
}

Area2D::Area2D(float i_x, float i_y, float i_w, float i_h)
: x(i_x), y(i_y), w(i_w), h(i_h)
{
}

Area2D::~Area2D()
{
}

void Area2D::SetArea2DByOrientationAndSize(
	AreaAlignOrientationEnum i_orientation,
	const Area2D &i_target_size,
	const Resolution &i_resolution)
{
	if (i_orientation == AreaAlignOrientation_LEFT_BOTTOM) {
		x = i_target_size.x / i_resolution.GetWidth();
		y = i_target_size.y / i_resolution.GetHeight();
		w = i_target_size.w / i_resolution.GetWidth();
		h = i_target_size.h / i_resolution.GetHeight();
	}
	else if (i_orientation == AreaAlignOrientation_RIGHT_BOTTOM) {
		x = 1.0f - ((i_target_size.x + i_target_size.w) / i_resolution.GetWidth());
		y = i_target_size.y / i_resolution.GetHeight();
		w = i_target_size.w / i_resolution.GetWidth();
		h = i_target_size.h / i_resolution.GetHeight();
	}
	else if (i_orientation == AreaAlignOrientation_LEFT_TOP) {
		x = i_target_size.x / i_resolution.GetWidth();
		y = 1.0f - ((i_target_size.y + i_target_size.h) / i_resolution.GetHeight());
		w = i_target_size.w / i_resolution.GetWidth();
		h = i_target_size.h / i_resolution.GetHeight();
	}
	else if (i_orientation == AreaAlignOrientation_RIGHT_TOP) {
		x = 1.0f - ((i_target_size.x + i_target_size.w) / i_resolution.GetWidth());
		y = 1.0f - ((i_target_size.y + i_target_size.h) / i_resolution.GetHeight());
		w = i_target_size.w / i_resolution.GetWidth();
		h = i_target_size.h / i_resolution.GetHeight();
	}
	else {
		SDLOGE("Error enum for area align orientation");
	}
}

vec2 Area2D::GetPoint(uint32_t i_orientation) const
{
	if (i_orientation == 0) {
		return vec2(x, y);
	}
	else if (i_orientation == 1) {
		return vec2(x + w, y);
	}
	else if (i_orientation == 2) {
		return vec2(x + w, y + h);
	}
	else if (i_orientation == 3) {
		return vec2(x, y + h);
	}
	else {
		return vec2::Zero;
	}
}

DepthArea2D::DepthArea2D()
: depth(0.0f)
{
}

DepthArea2D::~DepthArea2D()
{
}

vec3 DepthArea2D::GetPoint(uint32_t i_orientation) const
{
	vec2 pt = area.GetPoint(i_orientation);
	return vec3(pt.x, pt.y, depth);
}
______________SD_END_GRAPHICS_NAMESPACE______________