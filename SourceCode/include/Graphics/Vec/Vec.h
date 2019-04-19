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

/*! \file      Vec.h
 *  \brief     Vertex data struct.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/03/24
 *  \copyright MIT License.
 */
#pragma once

#include <string>

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

/*! \class vec3
 *  In our system, vec3 and vec2 is used to calculate and store some data such like vertex, normal, \n
 *  and texture coordinate. Class vec3 and vec2 are not only storing data but also offering some operator \n
 *  of vector. Programmer can calculate some necessary graphics data by those vector operators. I make a \n
 *  rule that all vertexs used to input to graphics API(vertices will be the parameter such like \n
 *  glVertex(x,y,z)) need to be composed by vec3 or vec2. Please note that vec classes can only \n
 *  used in graphics part. I don't allow program use it in general physical calculation and \n
 *  representation of position.It will make programmer confuse. I will use class (such like Vector3f) \n
 *  for these situations rather than vec. \n
 */
class vec3
{
public:
    /*! \fn static vec3 normalize(const vec3 &i_v);
     *  \param [in] i_v The target vec.
     *  \brief Normalize target vec to unit vec.
     */
    static vec3 normalize(const vec3 &i_v);
public:
    /*! \var static const vec3 Zero;
     *  \brief The zero vector. v(0.0, 0.0, 0.0)
     */
    static const vec3 Zero;
public:
    /*! \fn vec3();
     *  \brief Defualt constructor of vec3.
     */
    vec3():x(0.0f),y(0.0f),z(0.0f){}

    /*! \fn vec3(float i_x, float i_y, float i_z);
     *  \param [in] i_x x component.
     *  \param [in] i_y y component.
     *  \param [in] i_z z component.
     *  \brief Constructor of vec3.
     */
    vec3(float i_x,float i_y,float i_z):x(i_x),y(i_y),z(i_z){}

    /*! \fn vec3(const vec3 &i_src);
     *  \param [in] i_src target source.
     *  \brief Copy constructor of vec3.
     */
    vec3(const vec3 &i_src):x(i_src.x),y(i_src.y),z(i_src.z){}

    /*! \fn ~vec3();
     *  \brief Donstructor of vec3.
     */
    ~vec3(){}
//------------------------------ operator function ---------------------------
public:
    /*! \fn vec3 normalize() const;
     *  \brief Return the normalized vec3.
     */
    vec3 normalize() const;

    /*! \fn float dot(const vec3& i_b) const;
     *  \param [in] b target vec
     *  \brief dot operator.
     */
    float dot(const vec3 &i_b) const;

    /*! \fn float magnitude() const;
     *  \brief Return the magnitude of the vec.
     */
    float magnitude() const;

    /*! \fn vec3 cross(const vec3 &i_b) const;
     *  \param [in] b target vec
     *  \brief cross operator. Return vec that this cross b.
     */
    vec3 cross(const vec3 &i_b) const;

    /*! \fn vec3 scale(float i_s) const;
     *  \param [in] i_s scale factor.
     *  \brief Scale the vec or position.
     */
    vec3 scale(float i_s) const;

    /*! \fn vec3 operator+(const vec3 &i_b) const;
     *  \param [in] i_b target vec
     *  \brief Add operator.
     */
    vec3 operator+(const vec3 &i_b) const;

    /*! \fn vec3 operator-(const vec3 &i_b) const;
     *  \param [in] i_b target vec
     *  \brief Substract operator.
     */
    vec3 operator-(const vec3 &i_b) const;

    /*! \fn vec3& operator=(const vec3 &i_b);
     *  \param [in] i_b target vec
     *  \brief Assign operator.
     */
    vec3& operator=(const vec3 &i_b);

    /*! \fn vec3& operator+=(const vec3 &i_b);
     *  \param [in] i_b target vec.
     *  \brief Add equal operator.
     */
    vec3& operator+=(const vec3 &i_b);

    /*! \fn vec3& operator-=(const vec3 &i_b);
     *  \param [in] i_b target vector
     *  \brief Substract equal operator.
     */
    vec3& operator-=(const vec3 &i_b);

public://------------------------------ debug function ------------------------------
    /*! \fn std::string ToString() const;
     *  \brief Output to string.
     */
    std::string ToString() const;
public:
    /*! \var float x;
     *  \brief x component.
     */
    float x;

    /*! \var float y;
     *  \brief y component.
     */
    float y;

    /*! \var float z;
     *  \brief z component.
     */
    float z;
};

/*! \class vec2
    In our system, vec3 and vec2 is used to calculate and store some data such like vertex, normal, \n
    and texture coordinate. Class vec3 and vec2 are not only storing data but also offering some operator \n
    of vector. Programmer can calculate some necessary graphics data by those vector operators. I make a \n
    rule that all vertexs used to input to graphics API(vertices will be the parameter such like \n
    glVertex(x,y,z)) need to be composed by vec3 or vec2. Please note that vec classes can only \n
    used in graphics part. I don't allow program use it in general physical calculation and \n
    representation of position.It will make programmer confuse. I will use class (such like Vector3f) \n
    for these situations rather than vec. \n
*/
class vec2
{
public:
    /*! \fn static vec2 normalize(const vec2 &i_v);
     *  \param [in] i_v The target vec.
     *  \brief Normalize target vec to unit vec.
     */
    static vec2 normalize(const vec2 &i_v);
public:
    /*! \fn vec2();
     *  \brief Defualt constructor of vec2.
     */
    vec2():x(0.0f),y(0.0f){}

    /*! \fn vec2(float i_x,float i_y);
     *  \param [in] i_x x component.
     *  \param [in] i_y y component.
     *  \brief Constructor of vec2.
     */
    vec2(float i_x, float i_y):x(i_x),y(i_y){}
    
    /*! \fn vec2(const vec2 &i_src);
     *  \param [in] i_src target source.
     *  \brief Copy constructor of vec2.
     */
    vec2(const vec2 &i_src):x(i_src.x),y(i_src.y){}
    
    /*! \fn ~vec2();
     *  \brief Destructor of vec2.
     */
    ~vec2(){}
public://------------------------------ operator function ---------------------------
    /*! \fn vec2 normalize() const;
     *  \brief Return the normalized vec2.
     */
    vec2 normalize() const;

    /*! \fn float dot(const vec3& i_b) const;
     *  \param [in] b target vec
     *  \brief dot operator.
     */
    float dot(const vec2 &i_b) const;

    /*! \fn float magnitude() const;
     *  \brief Return the magnitude of the vec.
     */
    float magnitude() const;

    /*! \fn vec2 scale(float i_s) const;
     *  \param [in] i_s scale factor.
     *  \brief Scale the vec or position.
     */
    vec2 scale(float i_s) const;

    /*! \fn vec2 operator+(const vec2 &i_b) const;
     *  \param [in] i_b target vec
     *  \brief Add operator.
     */
    vec2 operator+(const vec2 &i_b) const;
    
    /*! \fn vec2 operator-(const vec2 &i_b) const;
     *  \param [in] i_b target vec
     *  \brief Substract operator.
     */
    vec2 operator-(const vec2 &i_b) const;

    /*! \fn vec2& operator=(const vec2 &i_b);
     *  \param [in] i_b target vec.
     *  \brief Assign operator.
     */
    vec2& operator=(const vec2 &i_b);

    /*! \fn vec2& operator+=(const vec2 &i_b);
     *  \param [in] i_b target vec.
     *  \brief Add equal operator.
     */
    vec2& operator+=(const vec2& i_b);
    
    /*! \fn vec2& operator-=(const vec2 &i_b);
     *  \param [in] i_b target vector
     *  \brief Substract equal operator.
     */
    vec2& operator-=(const vec2 &i_b);

    /*! \fn std::string ToString() const;
     *  \brief Output to string.
     */
    std::string ToString() const;
public:
    /*! \var float x;
     *  \brief x component.
     */
    float x;

    /*! \var float y;
     *  \brief y component.
     */
    float y;
};

//--------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}