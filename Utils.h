#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <cstdio>
#include <string>
#include <sstream>
#include <unistd.h>
#include <vector>

#include "Logger.h"

namespace Utils
{
	const int getBufferSize();
	const int CANT_OPEN_PIPE = 4;

	std::string myExec(const char* command);
	std::vector<std::string>& split(const std::string& input, 
									const char delimiter, 
									std::vector<std::string>& elements);
}

#endif // UTILS_H