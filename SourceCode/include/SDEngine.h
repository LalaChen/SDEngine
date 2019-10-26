#pragma once

/* ==============  SD Engine License ==============
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

/*! \file      SDEngine.h
 *  \brief     Total include header.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/01
 *  \copyright MIT License.
 */

//-------- Common Function ---------
#include "SDEngineCommonType.h"
#include "SDEngineMacro.h"
#include "SDEngineCommonFunction.h"
//-------- Math --------
#include "Matrix4X4f.h"
#include "Vector3f.h"
#include "Quaternion.h"
#include "Transform.h"
//-------- Basic --------
#include "Basic/Object/NullRefException.h"
#include "Basic/Object/Object.h"
#include "Basic/Object/StrongReferenceObject.h"
#include "Basic/Object/WeakReferenceObject.h"
#include "Basic/EventSystem/Event.h"
#include "Basic/EventSystem/EventArg.h"
#include "Basic/EventSystem/FunctionSlot.h"
#include "Basic/EventSystem/EventObject.h"
#include "Basic/Application/Application.h"
#include "Basic/LogManager/LogManager.h"
#include "Basic/Timer/Timer.h"
#include "Basic/FileSystem/FileSystemManager.h"
//-------- Graphics --------
#include "Graphics/Vec/Vec.h"
#include "Graphics/Vec/Color4f.h"
#include "Graphics/Resolution/Resolution.h"
#include "Graphics/Manager/GraphicsManager.h"
#include "Graphics/ImageLoader/Bitmap.h"
#include "Graphics/ImageLoader/ImageLoader.h"
#include "Graphics/ModelImporter/MaterialData.h"
#include "Graphics/ModelImporter/ModelData.h"
#include "Graphics/ModelImporter/MeshData.h"
#include "Graphics/ModelImporter/AssimpModelLoader.h"
#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Texture/Texture.h"
#include "Graphics/Material/BasicTextures.h"
#include "Graphics/VertexBuffer/DynamicVertexBuffer.h"
#include "Graphics/VertexBuffer/StaticVertexBuffer.h"
#include "Graphics/RenderFlow/ShaderModule.h"