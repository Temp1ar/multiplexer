#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include "ConfigFile.h"
#include "Logger.h"
#include "Utils.h"

class VNCServer 
{
public:
	VNCServer(const size_t ctId);

	bool deletePidFile() const;
	size_t getPort() const;
	bool run();
	bool isPidRunning() const;
	bool isRunning();
	bool updatePid();
	bool updatePort();
	
	static const size_t getBasePort();
	static const string getPathToServerExecutable();
	static const string getPidFilesDir();

private:
	size_t ctId_;
	std::string path_;
	size_t pid_;
	size_t port_;

	const size_t getBufferSize() const;
	const size_t getTimeout() const;
	const size_t getTimeStep() const;
};

#endif // SERVER_H