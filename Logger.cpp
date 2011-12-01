#include "Logger.h"

using namespace std;

Logger::Logger() 
{
	openlog(Logger::getLoggerName(), LOG_PID, 0);
}

Logger::~Logger()
{
	closelog();
	destroy();
}

void Logger::debug(const string& message) const
{
	syslog(LOG_DEBUG, message.c_str());
}

void Logger::destroy() 
{
	delete instance_;
}

void Logger::error(const string& message) const
{
	syslog(LOG_ERR, message.c_str());
}

Logger* Logger::getInstance() 
{
	if(instance_ == 0) {
		return instance_ = new Logger();
	} else {
		return instance_;
	}
}

void Logger::info(const string& message) const
{
	syslog(LOG_INFO, message.c_str());
}

void Logger::notice(const string& message) const
{
	syslog(LOG_NOTICE, message.c_str());
}

void Logger::warning(const string& message) const
{
	syslog(LOG_WARNING, message.c_str());
}

const char* Logger::getLoggerName()
{
	return "mplex";
}	

Logger* Logger::instance_ = 0;