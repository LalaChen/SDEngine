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

#include "AssimpLogWarpper.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

AssimpLogWarpper::AssimpLogWarpper(Assimp::Logger::LogSeverity i_severity)
: Assimp::Logger(i_severity)
{
    
}

AssimpLogWarpper::~AssimpLogWarpper()
{
}

bool AssimpLogWarpper::attachStream(Assimp::LogStream *i_stream, unsigned int i_severity)
{
    return true;
}

bool AssimpLogWarpper::detatchStream(Assimp::LogStream *i_stream, unsigned int i_severity)
{
    return true;
}

void AssimpLogWarpper::OnDebug(const char *i_message)
{
    SDLOGD("%s", i_message);
}

void AssimpLogWarpper::OnInfo(const char *i_message)
{
    SDLOG("%s", i_message);
}

void AssimpLogWarpper::OnWarn(const char *i_message)
{
    SDLOGW("%s", i_message);
}

void AssimpLogWarpper::OnError(const char *i_message)
{
    SDLOGE("%s", i_message);
}

______________SD_END_GRAPHICS_NAMESPACE______________