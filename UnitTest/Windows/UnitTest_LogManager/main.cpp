#include "SDEngine.h"
#include "WindowsLogManager.h"

using namespace SDE;
using namespace SDE::Basic;

//------------------------- Log ----------------------

class UnitTestLogManager : public WindowsLogManager
{
public:
    void LogToOutput(LogType i_type) override;
};

//construct for log.
void UnitTestLogManager::LogToOutput(LogType i_type)
{
    if (i_type == LogType::Normal)
    {
        std::cout << ((std::string("[Normal] ") + std::string(m_log_buffer) + std::string("\n")));
    }
    else if (i_type == LogType::Warning)
    {
        std::cout << ((std::string("[Warning] ") + std::string(m_log_buffer) + std::string("\n")));
    }
    else if (i_type == LogType::Internal)
    {
#ifndef _DEBUG
        //std::cout << "[Internal] " << m_log_buffer << std::endl;
#else
        std::cout << ((std::string("[Internal] ") + std::string(m_log_buffer) + std::string("\n")));
#endif
    }
    else
    {
        std::cout << ((std::string("[Error] ") + std::string(m_log_buffer) + std::string("\n")));
    }
}

int main(int argc, char **argv)
{
    std::cout << "UnitTest - LogManager" << std::endl;
    new UnitTestLogManager();
    SDLOG("This is LOG verbose(%d).", LogManager::LogType::Normal);
    SDLOGW("This is LOG warning(%d).", LogManager::LogType::Warning);
    SDLOGD("This is LOG internal(%d).", LogManager::LogType::Internal);
    SDLOGE("This is LOG error(%d).", LogManager::LogType::Error);
}
