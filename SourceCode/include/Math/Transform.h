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

/*! \file      Transform.h
 *  \brief     Transform class.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/09/14
 *  \copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "Matrix4X4f.h"
#include "Quaternion.h"
#include "Vector3f.h"

_______________SD_START_MATH_NAMESPACE_______________

/*! \class Transform
 *  In our engine, transform is used to record and describe the geometric
 *  information of scene object(creating at scene object ctor). Transform record two
 *  kind of geometric information. One is local coordinate system transformation(LCS),
 *  another is world coordinate system transformation(WCS). While we try to adjust LCS
 *  or WCS for one scene object, we will refresh transformation. What's more, we will
 *  refresh transform while the owner appended into other scene object(parent scene object).
 *  The transform in parent scene object will be set into child's shared reference.
 *  We will get local, world, and world normal matrix by transform. While making world
 *  transformation matrix, we will visit all ancestor(including itself) to get their LCS.
 *  Finally, we will get world transformation.
 *  
 *  <B><I>
 *  NOTE : For scale object, we need try our best to make model real. It's means
 *         we need to avoid setting scale factor. It will cause skewing.
 *  </I></B>
 */
class SDENGINE_CLASS Transform
{
public:
    /*! \fn static Transform DecomposeMatrixToTransform(const Matrix4X4f &i_src);
     *  \param [in] i_src target matrix.
     *  \brief Decompose matrix to translation, rotation and scale. Please note that it will cause error when decompose non-uniform-scale.
     */
    static Transform DecomposeMatrixToTransform(const Matrix4X4f &i_src);

    /*! \fn static Transform LookAt(const Vector3f &i_start, const Vector3f &i_end, const Vector3f &i_up);
     *  \param [in] i_start Start position.
     *  \param [in] i_focus focus on this position.
     *  \param [in] i_up Vup vector.
     *  \param [in] i_is_view Is making view transform object(forward is -z axis of object for right hand coordinate). Default false.
     *  \brief Make look at position.
     */
    static Transform LookAt(const Vector3f &i_start, const Vector3f &i_focus, const Vector3f &i_up, bool i_is_view = false);

public:
    /*! \fn Transform();
     *  \brief The constructor of Transform Class.
     */
    Transform();

    /*! \fn ransform(const Matrix4X4f &i_trans);
     *  \param [in] i_trans transformation matrix.
     *  \brief The constructor of Transform Class.
     */
    Transform(const Matrix4X4f &i_trans);

    /*! \fn ~Transform();
     *  \brief The destructor of Transform Class.
     */
    ~Transform();
    //================================== Matrix function  =================================
public:
    /*! \fn Matrix4X4f MakeWorldMatrix() const
     *  \brief Make transformation matrix(only transform, rotation, and scale).
     */
    Matrix4X4f MakeWorldMatrix() const;

    /*! \fn Matrix4X4f MakeViewMatrix() const
     *  \brief Make view matrix.
     */
    Matrix4X4f MakeViewMatrix() const;

    /*! \fn Matrix4X4f MakeNormalMatrix() const
     *  \brief Make normal matrix.
     */
    Matrix4X4f MakeNormalMatrix() const;

public:
    /*! \fn Vector3f GetForward() const;
     *  \brief Make +w of object space.
     */
    Vector3f GetForward() const;

    /*! \fn Vector3f GetRight() const;
     *  \brief Make +u of object space.
     */
    Vector3f GetRight() const;

    /*! \fn Vector3f GetTop() const;
     *  \brief Make +v of object space.
     */
    Vector3f GetTop() const;

public:
    /*! \fn void AddTranslation(const Vector3f &i_offset);
     *  \param [in] i_offset offset.
     *  \brief Add i_offset to postion.
     */
    void AddTranslation(const Vector3f &i_offset);

    /*! \fn void AddTranslation(const Vector3f &i_offset);
     *  \param [in] i_axis roation axis.
     *  \param [in] i_angle roation angle.
     *  \brief Rotation transfrom at current position via axis and angle. Angle is degree.
     */
    void AddRotation(const Vector3f &i_axis, float i_angle);

    //================================= Operator Function ==================================
public:
    /*! \fn Transform& operator=(const Transform &i_src);
     *  \param [in] i_src target transform.
     *  \brief assign operator.
     */
    Transform& operator=(const Transform &i_src);

    /*! \fn Transform& operator=(const Transform &i_b);
     *  \param [in] i_b target transform.
     *  \brief multiply operator.
     */
    Transform operator*(const Transform &i_b) const;

    /*! \fn Vector3f InverseVector3fToLocalSpace(const Vector3f &i_vec) const;
     *  \param [in] i_vec target vector3f.
     *  \brief Convert Vector or Position to local space.
     */
    Vector3f InverseVector3fToLocalSpace(const Vector3f &i_vec) const;
public:
    /*! \var Vector3f m_position;
     *  \brief variable recording position.
     */
    Vector3f m_position;

    /*! \var Quaternion m_rotation;
     *  \brief variable recording rotation.
     */
    Quaternion m_rotation;

    /*! \var Vector3f m_scale;
     *  \brief variable recording scale.
     */
    Vector3f m_scale;

    /*! \var Vector3f m_skew;
     *  \brief variable recording skew.
     */
    Vector3f m_skew;

    /*! \var Vector3f m_perspective;
     *  \brief variable recording perspective.
     */
    Vector3f m_perspective;
};

________________SD_END_MATH_NAMESPACE________________