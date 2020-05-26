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

/*! \file      SDEnginePlatform.h
 *  \brief     platform include header.
 *  \author    Kuan-Chih, Chen
 *  \date      2019/02/21
 *  \copyright MIT License.
 */

//------- OS Platform Only -------
#if defined(_WIN32) || defined(_WIN64) 
#include "Basic/LogManager/Windows/WindowsLogManager.h"
#include "Basic/FileSystem/Windows/WindowsFileSystemManager.h"
#include "Graphics/Manager/OpenGL4/OpenGL4Manager.h"
#include "Graphics/Manager/Vulkan/VulkanManager.h"
#include "Graphics/Manager/Vulkan/VulkanCreationArg.h"
#elif defined(__ANDROID__)
#include "Basic/LogManager/Android/AndroidLogManager.h"
#include "Basic/FileSystem/Android/AndroidAssetResourceManager.h"
#include "Graphics/Manager/Vulkan/VulkanManager.h"
#include "Graphics/Manager/Vulkan/VulkanCreationArg.h"
#endif