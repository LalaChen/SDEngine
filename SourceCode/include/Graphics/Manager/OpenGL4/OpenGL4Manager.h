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

/*! \file      OpenGL4Manager.h
 *  \brief     Introduce of class OpenGL4Manager
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/03
 *  \copyright MIT License.
 */
#pragma once

#include "GraphicsManager.h"

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

/*! \class OpenGL4Manager
 *  In our system, OpenGL4Manager is a implementation for opengl4 graphics API.
 */
class SDENGINE_CLASS OpenGL4Manager : public GraphicsManager
{
public:
	/*! \fn OpenGL4Manager();
	 *  \brief The constructor of OpenGL4Manager Class.
	 */
	OpenGL4Manager();
	
	/*! \fn virtual ~OpenGL4Manager();
	 *  \brief The destructor of OpenGL4Manager Class.
	 */
	virtual ~OpenGL4Manager();
public:
	/*! \fn void InitializeGraphicsSystem() override;
	 *  \param [in] i_arg Nothing. implement for following interface.
	 *  \brief Initialize graphics API. (link dll, ...)
	 */
	void InitializeGraphicsSystem(const EventArg &i_arg) override;
	
	/*! \fn void ReleaseGraphicsSystem() override;
	 *  \brief Release graphics API.
	 */
	void ReleaseGraphicsSystem() override;
};

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}