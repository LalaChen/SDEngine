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

#include "BasicShapeCreator.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________
//-------------------------- static part ---------------------------
vec3 BasicShapeCreator::CalculateTangetVector(const vec3& i_v1, const vec3& i_v2, const vec2& i_st1, const vec2& i_st2)
{
	vec3 tangent;
	vec3 tan_a, tan_b;
	float det = i_st1.x * i_st2.y - i_st2.x * i_st1.y;
	//tan_a
	tan_a.x = (i_st2.y * i_v1.x - i_v2.x * i_st1.y) / det;
	tan_a.y = (i_st2.y * i_v1.y - i_v2.y * i_st1.y) / det;
	tan_a.z = (i_st2.y * i_v1.z - i_v2.z * i_st1.y) / det;
	//tan_b
	tan_b.x = (i_st1.x * i_v2.x - i_v1.x * i_st2.x) / det;
	tan_b.y = (i_st1.x * i_v2.y - i_v1.y * i_st2.x) / det;
	tan_b.z = (i_st1.x * i_v2.z - i_v1.z * i_st2.x) / det;

	tangent = vec3::normalize(tan_a + tan_b);

	return tangent;
}

//------------------------------------------------------
SD_SINGLETON_DECLARATION_IMPL(BasicShapeCreator);

BasicShapeCreator::BasicShapeCreator()
{
}

BasicShapeCreator::~BasicShapeCreator()
{
}

______________SD_END_GRAPHICS_NAMESPACE______________