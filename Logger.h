#ifndef LOGGER_H
#define LOGGER_H

#include <syslog.h>
#include <string>

class Logger 
{
public:
	void debug(const std::string& message) const;
	void destroy();	
	void error(const std::string& message) const;
	static Logger* getInstance();
	void info(const std::string& message) const;
	void notice(const std::string& message) const;
	void warning(const std::string& message) const;
	
private:
	static Logger* instance_;
	static const char* getLoggerName();

	Logger();
	~Logger();
};



#endif // LOGGER_H