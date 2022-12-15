/*==============  SD Engine License ==============
MIT License

Copyright(c) 2019 Kuan - Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/*! \file      AppStateEnum.h
 *  \brief     Introduce of class AppStateEnum.
 *  \author    Kuan-Chih, Chen
 *  \date      2022/09/03
 *  \copyright MIT License.
 */

#pragma once

________________SD_START_APP_NAMESPACE_______________

enum AppStateEnum {
    AppState_CREATE = 0,
    AppState_INITIALIZE,
    AppState_INITIALIZE_GRAPHICS,
    AppState_RUN,
    AppState_PAUSE,
    AppState_TERMINATE,
    AppState_MAX_DEFINE_VALUE
};

_________________SD_END_APP_NAMESPACE________________