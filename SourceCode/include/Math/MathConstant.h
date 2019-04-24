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

/*! \file      MathConstant.h
 *  \brief     declare all math constant.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/04/21
 *  \copyright MIT License.
 */

//-------------------------- start of namespace SDE ---------------------------
namespace SDE
{
//-------------------------- start of namespace Math --------------------------
namespace Math
{

/*! \var const float PI;
 *  \brief PI
 */
const float PI = 3.1415926f;

/*! \var const float TWO_PI;
 *  \brief 2PI.
 */
const float TWO_PI = 6.2831852f;

/*! \var const float ONE_OVER_PI;
 *  \brief 1 / PI.
 */
const float ONE_OVER_PI = 0.3183099f;

/*! \var const float TWO_OVER_PI;
 *  \brief 1 / (2 * PI).
 */
const float TWO_OVER_PI = 0.1591549f;

/*! \var const float ONE_DEGREE_OF_PI;
 *  \brief PI / 180.0f. For degree converting.
 */
const float ONE_DEGREE_OF_PI = 0.0174533f; //PI / 180.0f

/*! \var const float FLOATING_ERROR;
 *  \brief floating error.
 */
const float FLOATING_ERROR = 0.000001f;

/*! \var const float FULL_DEGREE;
 *  \brief 1 circle = 360.0.
 */
const float FULL_DEGREE = 360.0f;

//--------------------------- end of namespace Math ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}