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

#include "Vector3f.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Math ----------------------------
namespace Math
{

/*! \class Matrix4X4f
    Class Matrix4X4f is used to represent the transformation of objects in 3D space. Please note that \n
    the Matrix4X4f is column-major. (glm::mat4)
*/
class Matrix4X4f
{
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