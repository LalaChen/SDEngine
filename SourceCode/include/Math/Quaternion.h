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

/*! \file      Quaternion.h
 *  \brief     Quaternion
 *  \author    Kuan-Chih, Chen
 *  \date      2019/04/20
 *  \copyright MIT License.
 */

#pragma once

#include <string>

#include <glm/gtx/quaternion.hpp>

#include "Matrix4X4f.h"
#include "Vector3f.h"

//-------------------------- start of namespace SDE ---------------------------
namespace SDE
{
//-------------------------- start of namespace Math --------------------------
namespace Math
{
/*! \class Quaternion
    Class Quaternion is used to represent rotation in 3D space.
 */
class Quaternion
{
public:
    /*! \fn static float Angle(const Quaternion &i_a, const Quaternion &i_b);
     *  \param [in] i_a start quaternion.
     *  \param [in] i_b end quaternion.
     *  \brief Return angle between quat a and b.
     */
    static float Angle(const Quaternion &i_a, const Quaternion &i_b);

    /*! \fn static Quaternion Slerp(const Quaternion &i_src, const Quaternion &i_dest, float i_ratio);
     *  \param [in] i_src start quaternion.
     *  \param [in] i_dest destination quaternion.
     *  \param [in] i_ratio weight.
     *  \brief Return angle between quat a and b.
     */
    static Quaternion Slerp(const Quaternion &i_src, const Quaternion &i_dest, float i_ratio);

    /*! \fn static Quaternion Lerp(const Quaternion &i_src, const Quaternion &i_dest, float i_ratio);
     *  \param [in] i_src start quaternion.
     *  \param [in] i_dest destination quaternion.
     *  \param [in] i_ratio weight.
     *  \brief Return angle between quat a and b.
     */
    static Quaternion Lerp(const Quaternion &i_src, const Quaternion &i_dest, float i_ratio);

    /*! \fn static Quaternion ConvertFromMatrix(const Matrix4X4f &i_mat)
     *  \param [in] i_mat target matrix.
     *  \brief Return quaternion converted matrix.
     */
    static Quaternion ConvertFromMatrix(const Matrix4X4f &i_mat);

public:
    /*! \fn Quaternion();
     *  \brief Default constructor.
     */
    Quaternion();

    /*! \fn Quaternion(const Quaternion &i_src);
     *  \param [in] i_src target quaternion.
     *  \brief copy constructor.
     */
    Quaternion(const Quaternion &i_src);

    /*! \fn Quaternion(const Vector3f &i_axis, float i_angle);
     *  \param [in] i_axis Rotate Axis.
     *  \param [in] i_angle Rotate Angle. It's degree.
     *  \brief Axis-Angle constructor.
     */
    Quaternion(const Vector3f &i_axis, float i_angle);

    /*! \fn Quaternion(float i_x_angle,float i_y_angle,float i_z_angle);
     *  \param [in] i_x_angle Rotate Angle. It's degree.
     *  \param [in] i_y_angle Rotate Angle. It's degree.
     *  \param [in] i_z_angle Rotate Angle. It's degree.
     *  \brief Euler constructor.
     */
    Quaternion(float i_x_angle, float i_y_angle, float i_z_angle);

    /*! \fn Quaternion(const glm::quat &i_quat);
     *  \param [in] i_quat target quat.
     *  \brief constructor.
     */
    Quaternion(const glm::quat &i_quat);

    /*! \fn ~Quaternion();
     *  \brief The destructor.
     */
    ~Quaternion();

public://operator
    /*! \fn Quaternion operator* (const Quaternion &b) const;
     *  \param [in] b target quaterion.
     *  \brief The multiply operation.
     */
    Quaternion operator* (const Quaternion &b) const;

    /*! \fn Quaternion& operator= (const Quaternion &b);
     *  \param [in] b target quaterion.
     *  \brief assign operator.
     */
    Quaternion& operator= (const Quaternion &b);

    /*! \fn Quaternion& operator*= (const Quaternion &b);
     *  \param [in] b target quaterion.
     *  \brief multiply and assign operator.
     */
    Quaternion& operator*= (const Quaternion &b);

    /*! \fn Quaternion inverse() const;
     *  \brief return inverse quaternion.
     */
    Quaternion inverse() const;

    /*! \fn Quaternion conjugation() const;
     *  \brief return conjugation quaternion.
     */
    Quaternion conjugation() const;

    /*! \fn Vector3f rotate (const Vector3f &b) const;
     *  \param [in] b target vector.
     *  \brief rotate b vector by this quaternion.
     */
    Vector3f rotate(const Vector3f &b) const;

    /*! \fn Vector3f toEulerianAngles() const;
     *  \brief convert quaternion to Eulerian Angles.
     */
    Vector3f toEulerianAngles() const;

public:
    /*! \fn std::string ToString() const;
     *  \brief to string.
     */
    std::string ToString() const;

    /*! \fn const float* GetDataAddr() const;
     *  \brief return raw pointer.
     */
    const float* GetDataAddr() const;

public:
    /*! \var glm::quat m_quat;
     *  \brief quat data.
     */
    glm::quat m_quat;
};

//--------------------------- end of namespace Math ----------------------------
}
//--------------------------- end of namespace SDE -----------------------------
}