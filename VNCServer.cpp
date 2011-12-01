#include "VNCServer.h"

using namespace std;

VNCServer::VNCServer(const size_t ctId) :
	ctId_(ctId)
{
	stringstream tmp;
	tmp << getPidFilesDir() << "/";
	tmp << "vncsvr" << ctId << ".pid";
	path_ = tmp.str();
	Logger::getInstance()->debug("The CTID servers file should be in:");
	Logger::getInstance()->debug(path_);
	pid_ = 0;
	updatePort();
}

bool VNCServer::deletePidFile() const 
{
	Logger::getInstance()->info("Deleting inactual pid file");
	return (unlink(path_.c_str()) == 0);
}

size_t VNCServer::getPort() const 
{
	return port_;
}

bool VNCServer::run() 
{	
	stringstream message;
	message << "Starting instance of server for CTID: " << ctId_ 
			<< " and with port: " << port_;
	Logger* Log = Logger::getInstance();
	Log->info(message.str());

	stringstream command;

	command << getPathToServerExecutable() <<  
			" -c " << ctId_ << " -p " << port_
			<< " > /dev/null 2> /dev/null &";
	Utils::myExec(command.str().c_str());

	// So we should wait until it loads in cycle for maximum timeout() ms
	size_t counter = 0;
	const size_t step = getTimeStep();
	while (counter < getTimeout() && !isRunning()) {
		usleep(step * 1000);
		counter += step;
	}

	if(counter < getTimeout()) {
		Log->info("Server successfully started");
		message << "Forked process has pid: " << pid_;
		Log->info(message.str());
	}
	else {
		Log->error("Timeout reached while starting server");
	}
	// This code repeated in Container class, maybe it will be better to 
	// extract class of method, use Runnable tmpl. ?
	
	return (pid_ != 0);
}

bool VNCServer::isPidRunning() const 
{
	Logger::getInstance()->info("Checking is process with pid running.");
	
	if(pid_ == 0) {
		return false;
	}

	ifstream proc;
	stringstream procPath;
	procPath << "/proc/" << pid_ << "/status";
	proc.open(procPath.str().c_str(), ifstream::in);
	proc.close();

	return !proc.fail();
}

bool VNCServer::isRunning() 
{
	Logger* Log = Logger::getInstance();
	if(updatePid()) {
		Log->info("Pid file already exist.");
		if(isPidRunning()) {
			Log->info("Server already running.");
		} 
		else {
			Log->info("Server not running.");
			if (!deletePidFile()) {
				Log->error("Can't delete inactual pid file.");
			}
			pid_ = 0;
		}
	}

	return (pid_ != 0);
}

bool VNCServer::updatePid() 
{
	Logger* Log = Logger::getInstance();
	ifstream input;
	input.open(path_.c_str(), ifstream::in);
	
	if(!input.fail()) {
		char buffer[getBufferSize()];
		input.get(buffer, getBufferSize());
		
		pid_ = atoi(buffer);
		stringstream message;
		message << "Getting pid from file: " << pid_;
		Log->info(message.str());
	} else {
		Log->info("File with such CTID doesn't exist.");
	}
	
	input.close();

	return (pid_ != 0);
}

bool VNCServer::updatePort() 
{
	port_ = getBasePort() + ctId_;
	return true;
}

const size_t VNCServer::getBasePort()
{
	return ConfigFile::getInstance()->read<size_t> ("server_baseport", 9000);
}

const size_t VNCServer::getBufferSize() const
{
	return 255;
}

const string VNCServer::getPathToServerExecutable()
{
	return ConfigFile::getInstance()->read<string> ("server_executable", "/bin/vncsvr");
}

const string VNCServer::getPidFilesDir()
{
	return ConfigFile::getInstance()->read<string> ("server_pidFilesDir", "/var/run/");
}	

const size_t VNCServer::getTimeout() const
{
	return ConfigFile::getInstance()->read<size_t> ("server_timeout", 5000);
}

const size_t VNCServer::getTimeStep() const
{
	return ConfigFile::getInstance()->read<size_t> ("server_timeStep", 500);
}
