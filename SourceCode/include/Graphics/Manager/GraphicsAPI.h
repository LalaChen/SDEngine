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

/*! \file      GraphicsAPI.h
	\brief     Introduce of class GraphicsAPI
	\author    Kuan-Chih, Chen
	\date      2019/02/03
	\copyright MIT License.
 */

#pragma once

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "SDEngineCommonFunction.h"

using SDE::Graphics::Size_ui;

//---------------------------- start of namespace SDE ----------------------------
namespace SDE
{
//---------------------------- start of namespace Graphics ----------------------------
namespace Graphics
{
/*! \class GraphicsAPI
	In our system, GraphicsAPI is a interface for all graphics API(opengl, gles, vulkan). \n
	We will integrate some APIs to a behavior(like create texture, vertex buffer, ... etc.)
*/
class SDENGINE_CLASS GraphicsAPI
{
public:
	SINGLETON_DECLARATION(GraphicsAPI);
public:
	/*! \fn explicit GraphicsAPI();
	    \brief The constructor of GraphicsAPI Class. 
	*/
	explicit GraphicsAPI();

	/*! \fn virtual ~GraphicsAPI();
		\brief The destructor of GraphicsAPI Class.
	*/
	virtual ~GraphicsAPI();
public: //---------------- Initialize and Release -----------------
	/*! \fn virtual void InitializeGraphicsSystem() = 0;
		\brief Initialize graphics API. (link dll, ...)
	*/
	virtual void InitializeGraphicsSystem() = 0;

	/*! \fn virtual void ReleaseGraphicsSystem() = 0;
		\brief Release graphics API.
	*/
	virtual void ReleaseGraphicsSystem() = 0;

public: //---------------- API -----------------
};

//---------------------------- end of namespace Graphics ----------------------------
}
//---------------------------- end of namespace SDE ----------------------------
}