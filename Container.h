#ifndef CONTAINER_H
#define CONTAINER_H

#include <cstring> 
#include <sstream>
#include <string>
#include <vector>

#include "ConfigFile.h"
#include "Logger.h"
#include "Utils.h"

class Container 
{
public:
	Container(const size_t ctId);

	bool run();
	bool isExist() const;
	bool isRunning() const;
	void updateStatus();

private:
	size_t ctId_;
	bool isExist_;
	bool isRunning_;

	size_t getTimeout() const;
	size_t getTimeStep() const;
};

#endif // CONTAINER_H