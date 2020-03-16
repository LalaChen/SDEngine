#if defined(__ANDROID__) 

#include <android/log.h>

#include "AndroidLogManager.h"

#define  LOG_TAG    "SDEngine"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

______________SD_START_BASIC_NAMESPACE_______________

AndroidLogManager::AndroidLogManager(void)
{
}

AndroidLogManager::~AndroidLogManager(void)
{
}

void AndroidLogManager::Log(LogType i_type, const std::string &i_prefix, const char *i_log , ...)
{
	m_prefix = i_prefix;
	va_list args;
	va_start(args, i_log);
	vsnprintf(m_log_buffer, SD_MAX_WORD_PER_LINE - 3 , i_log, args);
    va_end(args);
	LogToOutput(i_type);
}

void AndroidLogManager::Log(LogType i_type, const std::string &i_prefix, const char *i_log , va_list i_args)
{
	m_prefix = i_prefix;
	vsnprintf(m_log_buffer, SD_MAX_WORD_PER_LINE - 3 , i_log, i_args);
	LogToOutput(i_type);
}

void AndroidLogManager::LogToOutput(LogType i_type)
{
	if(i_type == LogType::Normal)
	{
		LOGI("[Normal] %s%s",m_prefix.c_str(), m_log_buffer);
	}
	else if(i_type == LogType::Warning)
	{
		LOGW("[Warning] %s%s",m_prefix.c_str(), m_log_buffer);
	}
	else if (i_type == LogType::Internal)
	{
        LOGW("[Internal] %s%s",m_prefix.c_str(), m_log_buffer);
	}
	else
	{
		LOGE("[Error] %s%s",m_prefix.c_str(), m_log_buffer);
	}
}

_______________SD_END_BASIC_NAMESPACE________________

#endif