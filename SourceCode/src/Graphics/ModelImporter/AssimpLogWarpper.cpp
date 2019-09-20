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