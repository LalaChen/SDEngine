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

/*! \file      OpenGL4API.h
 *  \brief     Introduce of class OpenGL4API
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/03
 *  \copyright MIT License.
 */

#include "Graphics.h"

 //---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{

/*! \class OpenGL4
 *  In our system, OpenGL4 is a implementation for opengl4 graphics API and manage graphics some resource.
 */
class SDENGINE_CLASS OpenGL4 : public Graphics
{
public:
	/*! \fn OpenGL4();
	 *  \brief The constructor of OpenGL4 Class.
	 */
	OpenGL4();
	
	/*! \fn virtual ~GraphicsAPI();
	 *  \brief The destructor of OpenGL4 Class.
	 */
	virtual ~OpenGL4();
public:
	/*! \fn void InitializeGraphicsSystem() override;
	 *  \brief Initialize graphics API. (link dll, ...)
	 */
	void InitializeGraphicsSystem() override;
	
	/*! \fn void ReleaseGraphicsSystem() override;
	 *  \brief Release graphics API.
	 */
	void ReleaseGraphicsSystem() override;
};

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}