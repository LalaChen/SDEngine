/* ==============  SD Engine License ==============
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

#include <cmath>
#include "MathConstant.h"
#include "Vector3f.h"
#include "MathAlgoritm.h"

_______________SD_START_MATH_NAMESPACE_______________

bool CalculateRayTriangleIntersection(
    float &io_distance, float &io_u, float &io_v, Vector3f &io_hitted_point,
    const Vector3f &i_v0, const Vector3f &i_v1, const Vector3f &i_v2,
    const Vector3f &i_ray_origin, const Vector3f &i_ray_direction, const float i_length,
    bool i_culling)
{
    Vector3f v0v1 = i_v1 - i_v0; v0v1.RepresentVector();
    Vector3f v0v2 = i_v2 - i_v0; v0v2.RepresentVector();
    Vector3f pvec = i_ray_direction.cross(v0v2);
    float det = v0v1.dot(pvec);
    if (i_culling == true) {
        if (det < FLOATING_ERROR) {
            return false;
        }
    }
    else {
        if (std::fabs(det) < FLOATING_ERROR) {
            return false;
        }
    }

    float inv_det = 1 / det;

    Vector3f tvec = i_ray_origin - i_v0;
    io_u = tvec.dot(pvec) * inv_det;
    if (io_u < 0 || io_u > 1) return false;

    Vector3f qvec = tvec.cross(v0v1);
    io_v = i_ray_direction.dot(qvec) * inv_det;
    if (io_v < 0.0f || io_u + io_v > 1.0f) return false;

    io_distance = v0v2.dot(qvec) * inv_det;

    io_hitted_point = i_v0 + v0v1.scale(io_u) + v0v2.scale(io_v);

    if (i_length <= 0.0f) {
        return true;
    }
    else {
        return (io_distance <= i_length);
    }
}

________________SD_END_MATH_NAMESPACE________________