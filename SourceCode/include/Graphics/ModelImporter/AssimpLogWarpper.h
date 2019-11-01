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

/*! \file      AssimpLogWarpper.h
 *  \brief     Introduce of class AssimpLogWarpper.
 *  \author    Kuan-Chih, Chen
 *  \date      2018/09/20
 *  \copyright MIT Public License.
 */

#pragma once

#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>

#include "SDEngineMacro.h"
#include "SDEngineCommonType.h"
#include "LogManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

/*! \class AssimpLogWarpper
 *  \brief AssimpLogWarpper is used to hook the logger system of assimp for printing log in assimp.
 */
class SDENGINE_CLASS AssimpLogWarpper : public Assimp::Logger
{
public:
    /*! \fn explicit AssimpLogWarpper(Assimp::Logger::LogSeverity severity = LogSeverity::VERBOSE);
     *  \param severity parameter of Assimp.
     *  \brief Ctor of AssimpLogWarpper.
     */
    explicit AssimpLogWarpper(Assimp::Logger::LogSeverity severity = LogSeverity::VERBOSE);

    /*! \fn ~AssimpLogWarpper();
     *  \brief Dtor of AssimpLogWarpper.
     */
    ~AssimpLogWarpper();
public:
    /*! \fn bool attachStream(Assimp::LogStream *i_stream, unsigned int i_severity = Debugging | Err | Warn | Info) override;
     *  \param [in] i_stream Logger stream of assimp.
     *  \param [in] i_severity severity of assimp.
     *  \brief Attach stream in this logger.
     */
    bool attachStream(Assimp::LogStream *i_stream, unsigned int i_severity = Debugging | Err | Warn | Info) override;

    /*! \fn bool detatchStream(Assimp::LogStream *i_stream, unsigned int i_severity = Debugging | Err | Warn | Info) override;
     *  \param [in] i_stream Logger stream of assimp.
     *  \param [in] i_severity severity of assimp.
     *  \brief Detach stream in this logger.
     */
    bool detatchStream(Assimp::LogStream *i_stream, unsigned int i_severity = Debugging | Err | Warn | Info) override;
protected:

    /*! \fn void OnDebug(const char *i_message) override;
     *  \param [in] i_message Message for logging.
     *  \brief Print log for debugging.
     */
    void OnDebug(const char *i_message) override;

    /*! \fn void OnInfo(const char *i_message) override;
     *  \param [in] i_message Message for logging.
     *  \brief Print log for showing information.
     */
    void OnInfo(const char *i_message) override;

    /*! \fn void OnWarn(const char *i_message) override;
     *  \param [in] i_message Message for logging.
     *  \brief Print log for warning.
     */
    void OnWarn(const char *i_message) override;

    /*! \fn void OnError(const char *i_message) override;
     *  \param [in] i_message Message for logging.
     *  \brief Print log for showing error.
     */
    void OnError(const char *i_message) override;
};
______________SD_END_GRAPHICS_NAMESPACE______________