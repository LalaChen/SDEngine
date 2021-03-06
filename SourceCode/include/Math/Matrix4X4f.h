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

/*! \file      Matrix4X4f.h
 *  \brief     4X4 matrix.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/04/18
 *  \copyright MIT License.
 */

#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "SDEngineMacro.h"

_______________SD_START_MATH_NAMESPACE_______________

class Quaternion;
class Vector3f;

/*! \class Matrix4X4f
 *  Class Matrix4X4f is used to represent the transformation of objects in 3D space. Please note that
 *  the Matrix4X4f is column-major. (glm::mat4)
 */
class SDENGINE_CLASS Matrix4X4f
{
public:
    /*! \fn static bool decompose(const Matrix4X4f &i_mat, Vector3f &io_scale, Quaternion &io_rot, Vector3f &io_skew, Vector3f &io_translation);
     *  \param [in] i_mat target matrix.
     *  \param [inout] io_scale the scale factor decomposed from matrix.
     *  \param [inout] io_rot the rotation decomposed from matrix.
     *  \param [inout] io_skew the sharing decomposed from matrix.
     *  \param [inout] io_translation the io_translation decomposed from matrix.
     *  \param [inout] io_prespective the io_prespective decomposed from matrix.
     *  \brief decompose matrix to transformation.
     */
    static bool decompose(const Matrix4X4f &i_mat, Vector3f &io_scale, Quaternion &io_rot, Vector3f &io_skew, Vector3f &io_translation, Vector3f &io_prespective);
public:
    /*! \fn Matrix4X4f();
     *  \brief Default constructor. Initialize matrix by identity.
     */
    Matrix4X4f();

    /*! \fn Matrix4X4f(float i_datas[]);
     *  \param [in] i_data.
     *  \brief array constructor.
     */
    Matrix4X4f(float i_datas[]);

    /*! \fn Matrix4X4f(const Matrix4X4f &i_src);
     *  \param [in] i_src target matrix
     *  \brief Copy constructor.
     */
    Matrix4X4f(const Matrix4X4f &i_src);

    /*! \fn Matrix4X4f(const glm::mat &i_src);
     *  \param [in] i_src target matrix
     *  \brief Copy constructor.
     */
    Matrix4X4f(const glm::mat4 &i_src);

    /*! \fn Matrix4X4f();
     *  \brief Destructor.
     */
    ~Matrix4X4f();

public: //operator
    /*! \fn Matrix4X4f& operator=(const Matrix4X4f &i_src);
     *  \param [in] i_src target matrix.
     *  \brief Assign operator.
     */
    Matrix4X4f& operator=(const Matrix4X4f &i_src);

    /*! \fn Matrix4X4f& operator*=(const Matrix4X4f &i_src);
     *  \param [in] i_src target matrix.
     *  \brief *= operator.
     */
    Matrix4X4f& operator*=(const Matrix4X4f &i_src);

    /*! \fn Matrix4X4f operator*(const Matrix4X4f &i_src) const;
     *  \param [in] i_src target matrix.
     *  \brief * operator.
     */
    Matrix4X4f operator*(const Matrix4X4f &i_src) const;

    /*! \fn Vector3f operator*(const Vector3f &i_src) const;
     *  \param [in] i_src target position
     *  \brief * operator for transforming the i_src.
     */
    Vector3f operator*(const Vector3f &i_src) const;

    /*! \fn Matrix4X4f inverse() const;
     *  \brief return inverse matrix.
     */
    Matrix4X4f inverse() const;

    /*! \fn Matrix4X4f transpose() const;
     *  \brief return transpose matrix.
     */
    Matrix4X4f transpose() const;
//--------------- World Space Using --------------------
    /*! \fn void translate(const Vector3f &i_trans);
     *  \param [in] i_trans set translation.
     *  \brief translate matrix. 
     */
    void translate(const Vector3f &i_trans);

    /*! \fn void rotate(const Quaternion &i_rotate);
     *  \param [in] i_rotate set rotate matrix.
     *  \brief rotate matrix.
     */
    void rotate(const Quaternion &i_rotate);

    /*! \fn void scale(float i_scale);
     *  \param [in] i_scale set scale factor.
     *  \brief translate matrix.
     */
    void scale(float i_scale);

    /*! \fn void scale(const Vector3f &i_scale);
     *  \param [in] i_scale set scale factor for (x,y,z).
     *  \brief scale matrix.
     */
    void scale(const Vector3f &i_scale);
//----------------- View Space Using ---------------------
    /*! \fn void lookAt(const Vector3f &i_eye, const Vector3f &i_view_center, const Vector3f &i_up);
     *  \param [in] i_eye eye position.
     *  \param [in] i_view_center set view center.
     *  \param [in] i_up view up.
     *  \brief set view matrix.
     */
    void lookAt(const Vector3f &i_eye, const Vector3f &i_view_center, const Vector3f &i_up);

//--------------- Project Space Using --------------------
    /*! \fn void perspective(float i_fovy, float i_aspect, float i_near, float i_far);
     *  \param [in] i_fovy set angle.
     *  \param [in] i_aspect set ratio between w and h.
     *  \param [in] i_near near distance.
     *  \param [in] i_far far distance.
     *  \brief set perspective projection matrix.
     */
    void perspective(float i_fovy, float i_aspect, float i_near, float i_far);

    /*! \fn void ortho(float i_left, float i_right, float i_bottom, float i_top, float i_near, float i_far);
     *  \param [in] i_left x left.
     *  \param [in] i_right x right.
     *  \param [in] i_bottom set y buttom.
     *  \param [in] i_top y top.
     *  \param [in] i_near set z near.
     *  \param [in] i_far set z far.
     *  \brief set orthogonal projection matrix.
     */
    void ortho(float i_left, float i_right, float i_bottom, float i_top, float i_near, float i_far);

public:
    /*! \fn std::string ToString() const;
     *  \brief Return matrix.
     */
    std::string ToString() const;

    /*! \fn std::string ToFormatString(const std::string &i_mat_name, const std::string &i_prefix) const;
     *  \param [in] i_mat_name Name about matrix.
     *  \param [in] i_prefix Line about matrix.
     *  \brief Return matrix.
     */
    std::string ToFormatString(const std::string &i_mat_name, const std::string &i_prefix) const;

    /*! \fn const float* GetDataAddr() const;
     *  \brief return raw pointer.
     */
    const float* GetDataAddr() const;

public:
    /*! \var glm::mat4 m_matrix;
     *  \brief matrix data.
     */
    glm::mat4 m_matrix;
};

________________SD_END_MATH_NAMESPACE________________