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

#include <glm/glm.hpp>

//--------------------------- start of namespace SDE ---------------------------
namespace SDE
{
//-------------------------- start of namespace Math --------------------------
namespace Math
{

/*! \class Vector3f
    Class Vector3f is used to represent position or vector in 3D space. The different between the two \n
    is the w component. If this one is represented position, w should be equal to 1.0. Otherwise, the \n
    value is 0.0. We also provide a series of operator for convenience. Please note that the all operator \n
    don't consider w component. We will keep the w equal to 1.0(position) or 0.0(vector).
 */
class Vector3f
{
public:
    static Vector3f Zero;
public:
    /*! \fn Vector3f();
     *  \brief Default constructor. We will initialize this vector by zero vector.
     */
    Vector3f(): m_vec(glm::vec4(0.0, 0.0, 0.0, 0.0)) {}

    /*! \fn Vector3f(float i_x, float i_y, float i_z);
     *  \param [in] i_x x component.
     *  \param [in] i_y y component.
     *  \param [in] i_z z component.
     *  \brief Vector constructor. We will initialize this vector by (i_x, i_y, i_z).
     */
    Vector3f(float i_x, float i_y, float i_z): m_vec(glm::vec4(i_x, i_y, i_z, 0.0)) {}

    /*! \fn Vector3f(float i_x, float i_y, float i_z, float i_w);
     *  \param [in] i_x x component.
     *  \param [in] i_y y component.
     *  \param [in] i_z z component.
     *  \param [in] i_w w component.
     *  \brief The constructor. We will initialize this vector by (i_x, i_y, i_z, i_w).
     */
    Vector3f(float i_x, float i_y, float i_z, float i_w): m_vec(glm::vec4(i_x, i_y, i_z, i_w)) {}

    /*! \fn Vector3f(const Vector3f &i_src);
     *  \param [in] i_src target vector.
     *  \brief The copy constructor.
     */
    Vector3f(const Vector3f &i_src): m_vec(i_src.m_vec) {}

    /*! \fn ~Vector3f();
     *  \brief The destructor.
     */
    ~Vector3f() {}
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

    /*! \fn bool is_parallel(const Vector3f &b) const;
     *  \param [in] b target vector.
     *  \brief Return the result about whether a is parallel to b or not. \n
     *         If a or b is position, we will use its x,y,z to make unit vector.
     */
    bool is_parallel(const Vector3f &b) const;

    /*! \fn bool is_vertical(const Vector3f &b) const;
     *  \param [in] b target vector.
     *  \brief Return the result about whether a is vertical to b or not. \n
     *         If a or b is position, we will use its x,y,z to make unit vector.
     */
    bool is_vertical(const Vector3f &b) const;
private:
    /*! \fn Vector3f(const glm::vec4 &b);
     *  \brief Private constructor.
     */
    Vector3f(const glm::vec4 &b): m_vec(b) {}

    /*! \fn Vector3f(const glm::vec3 &b);
     *  \brief Private constructor.
     */
    Vector3f(const glm::vec3 &b): m_vec(b.x, b.y, b.z, 0.0f) {}
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