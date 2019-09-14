/*==============  SD Engine License ==============
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

/*! \file      AssimpModelLoader.h
 *  \brief     Introduce of class AssimpModelLoader.
 *  \author    Kuan-Chih, Chen
 *  \date      2018/09/14
 *  \copyright FreeBSD Public License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"

//--------------------------- start of namespace SDE ---------------------------
namespace SDE
{
//------------------------- start of namespace Graphics ------------------------
namespace Graphics
{

/*! \class GraphicsManager
 *  In our graphics system, class AssimpModelLoader is used to load model file and convert the one to
 *  model asset(ModelAsset). AssimpLoader is based on lib assimp. The loader will convert assimp 
 *  structure to Node structure.
 */
class SDENGINE_CLASS AssimpModelLoader
{
public:
    SD_SINGLETON_DECLARATION(AssimpModelLoader);
public:
    /*! \fn explicit AssimpModelLoader();
     *  \brief Default constructor about AssimpModelLoader.
     */
    AssimpModelLoader();

    /*! \fn ~AssimpModelLoader();
     *  \brief Destructor about AssimpModelLoader.
     */
    ~AssimpModelLoader();
};

//-------------------------- end of namespace Graphics -------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}