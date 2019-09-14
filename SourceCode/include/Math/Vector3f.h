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

/*! \file      Vector3f.h
 *  \brief     Vector3f.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/04/19
 *  \copyright MIT License.
 */

#pragma once

#include <string>

#include <glm/glm.hpp>

#include "SDEngineMacro.h"

//-------------------------- start of namespace SDE ---------------------------
namespace SDE
{
//-------------------------- start of namespace Math --------------------------
namespace Math
{

class Matrix4X4f;
class Quaternion;

/*! \class Vector3f
 *  Class Vector3f is used to represent position or vector in 3D space. The different between the two
 *  is the w component. If this one is represented position, w should be equal to 1.0. Otherwise, the
 *  value is 0.0. We also provide a series of operator for convenience. Please note that the all operator
 *  don't consider w component. We will keep the w equal to 1.0(position) or 0.0(vector).
 */
class SDENGINE_CLASS Vector3f
{
public:
    /*! \var static const Vector3f PositiveX;
     *  \brief Zero vector.
     */
    static const Vector3f PositiveX;

    /*! \var static const Vector3f PositiveY;
     *  \brief Zero vector.
     */
    static const Vector3f PositiveY;

    /*! \var static const Vector3f PositiveZ;
     *  \brief Zero vector.
     */
    static const Vector3f PositiveZ;

    /*! \var static const Vector3f Zero;
     *  \brief Zero vector.
     */
    static const Vector3f Zero;
public:
    /*! \fn Vector3f();
     *  \brief Default constructor. We will initialize this vector by zero vector.
     */
    Vector3f();

    /*! \fn Vector3f(float i_x, float i_y, float i_z);
     *  \param [in] i_x x component.
     *  \param [in] i_y y component.
     *  \param [in] i_z z component.
     *  \brief Vector constructor. We will initialize this vector by (i_x, i_y, i_z).
     */
    Vector3f(float i_x, float i_y, float i_z);

    /*! \fn Vector3f(float i_x, float i_y, float i_z, float i_w);
     *  \param [in] i_x x component.
     *  \param [in] i_y y component.
     *  \param [in] i_z z component.
     *  \param [in] i_w w component.
     *  \brief The constructor. We will initialize this vector by (i_x, i_y, i_z, i_w).
     */
    Vector3f(float i_x, float i_y, float i_z, float i_w);

    /*! \fn Vector3f(const Vector3f &i_src);
     *  \param [in] i_src target vector.
     *  \brief The copy constructor.
     */
    Vector3f(const Vector3f &i_src);

    /*! \fn Vector3f(const glm::vec4 &b);
     *  \brief Private constructor.
     */
    Vector3f(const glm::vec4 &b);

    /*! \fn Vector3f(const glm::vec3 &b);
     *  \brief Private constructor.
     */
    Vector3f(const glm::vec3 &b);

    /*! \fn ~Vector3f();
     *  \brief The destructor.
     */
    ~Vector3f();

public: //operator
    /*! \fn Vector3f& operator=(const Vector3f &b);
     *  \param [in] b target vector
     *  \brief Assign operator.
     */
    Vector3f& operator=(const Vector3f &b);

    /*! \fn Vector3f& operator+=(const Vector3f &b);
     *  \param [in] b target vector
     *  \brief Add equal operator. The final w component won't be modified.
     */
    Vector3f& operator+=(const Vector3f &b);

    /*! \fn Vector3f& operator-=(const Vector3f &b);
     *  \param [in] b target vector
     *  \brief Substract equal operator. The final w component won't be modified.
     */
    Vector3f& operator-=(const Vector3f &b);

    /*! \fn Vector3f operator-(const Vector3f &b) const;
     *  \param [in] b target vector
     *  \brief Substract operator. The final w component of result won't be modified.
     */
    Vector3f operator-(const Vector3f &b) const;

    /*! \fn Vector3f operator+(const Vector3f &b) const;
     *  \param [in] b target vector
     *  \brief Add operator. The final w component of result won't be modified.
     */
    Vector3f operator+(const Vector3f &b) const;

    /*! \fn float dot(const Vector3f &b) const;
     *  \param [in] b target vector
     *  \brief dot operator.
     */
    float dot(const Vector3f &b) const;

    /*! \fn Vector3f cross(const Vector3f &b) const;
     *  \param [in] b target vector
     *  \brief cross operator. Return vector that this cross b.
     */
    Vector3f cross(const Vector3f &b) const;

    /*! \fn Vector3f scale(float i_scale_factor) const;
     *  \param [in] i_scale_factor scale factor.
     *  \brief Scale the vector or position. We won't modify w.
     */
    Vector3f scale(float i_scale_factor) const;

    /*! \fn Vector3f scale(float i_sx, float i_sy, float i_sz) const;
	 *  \param [in] i_sx scale factor of component x.
	 *  \param [in] i_sy scale factor of component y.
	 *  \param [in] i_sz scale factor of component z.
	 *  \brief Scale the vector or position. We won't modify w.
	 */
	Vector3f scale(float i_sx, float i_sy, float i_sz) const;

    /*! \fn float magnitude() const;
     *  \brief Return the magnitude of the vector. Please note we don't consider w component.
     */
    float magnitude() const;

    /*! \fn Vector3f normalize() const;
	 *  \brief Return the normalized vector.
	 */
	Vector3f normalize() const;

	/*! \fn Vector3f negative() const;
	 *	\brief Return the negivate vector or position. We won't modify the w component of vector.
	 */
	Vector3f negative() const;

    /*! \fn bool operator==(const Vector3f &b) const;
     *  \param [in] b target vector.
     *  \brief Return the result about whether a is equal to b or not.
     */
    bool operator==(const Vector3f &b) const;

    /*! \fn bool parallel(const Vector3f &b) const;
     *  \param [in] b target vector.
     *  \brief Return the result about whether a is parallel to b or not. \n
     *         If a or b is position, we will use its x,y,z to make unit vector.
     */
    bool parallel(const Vector3f &b) const;

    /*! \fn bool vertical(const Vector3f &b) const;
     *  \param [in] b target vector.
     *  \brief Return the result about whether a is vertical to b or not. \n
     *         If a or b is position, we will use its x,y,z to make unit vector.
     */
    bool vertical(const Vector3f &b) const;

public:
    /*! \fn void RepresentPosition();
     *  \brief Modify the w component of this Vector3f to 1.0.
     */
    void RepresentPosition();

    /*! \fn void RepresentVector();
     *  \brief Modify the w component of this Vector3f to 0.0.
     */
    void RepresentVector();

    /*! \fn std::string ToString() const;
        \brief Return vector by format (%lf,%lf,%lf).
    */
    std::string ToString() const;

    /*! \fn const float* GetDataAddr() const;
     *  \brief return raw pointer.
     */
    const float* GetDataAddr() const;

public:
    /*! \var glm::vec4 m_vec;
     *  \brief vector data.
     */
    glm::vec4 m_vec;
};
//--------------------------- end of namespace Math ----------------------------
}
//--------------------------- end of namespace SDE -----------------------------
}