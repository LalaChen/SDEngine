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

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Math ----------------------------
namespace Math
{

class Quaternion;
class Vector3f;

/*! \class Matrix4X4f
    Class Matrix4X4f is used to represent the transformation of objects in 3D space. Please note that \n
    the Matrix4X4f is column-major. (glm::mat4)
*/
class SDENGINE_CLASS Matrix4X4f
{
public:
    /*! \fn static bool decompose(const Matrix4X4f &i_mat, Vector3f &io_scale, Quaternion &io_rot, Vector3f &io_skew, Vector3f &io_translation);
     *  \param [in] i_mat target matrix.
     *  \paran [inout] io_scale the scale factor decomposed from matrix.
     *  \paran [inout] io_rot the rotation decomposed from matrix.
     *  \paran [inout] io_skew the sharing decomposed from matrix.
     *  \paran [inout] io_translation the io_translation decomposed from matrix.
     *  \paran [inout] io_prespective the io_prespective decomposed from matrix.
     *  \brief decompose matrix to transformation.
     */
    static bool decompose(const Matrix4X4f &i_mat, Vector3f &io_scale, Quaternion &io_rot, Vector3f &io_skew, Vector3f &io_translation, Vector3f &io_prespective);
public:
    /*! \fn Matrix4X4f();
     *  \brief Default constructor. Initialize matrix by identity.
     */
    Matrix4X4f();

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
public:
    /*! \fn std::string ToString() const;
     *  \brief Return vector by format (%lf,%lf,%lf).
     */
    std::string ToString() const;

    /*! \fn const float *GetDataAddr() const;
     *  \brief return raw pointer.
     */
    const float *GetDataAddr() const;

public:
    /*! \var glm::mat4 m_matrix;
     *  \brief matrix data.
     */
    glm::mat4 m_matrix;
};

//--------------------------- end of namespace Math ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}