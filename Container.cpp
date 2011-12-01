#include "Container.h"

using namespace std;

Container::Container(const size_t ctId) :
	ctId_(ctId),
	isExist_(false),
	isRunning_(false)
{
	updateStatus();
}

size_t Container::getTimeout() const
{
	// How long should we wait until container is loading
	// in milliseconds
	return ConfigFile::getInstance()->read<size_t> ("container_timeout", 5000);
}

size_t Container::getTimeStep() const
{
	// Step, after which we checking if container is running or not
	// should be smaller then getTimeout() constant
	// in milliseconds
	return ConfigFile::getInstance()->read<size_t> ("container_timeStep", 500);
}

bool Container::run() 
{
	Logger *Log = Logger::getInstance();

	stringstream command;
	command << "vzctl start ";
	command << ctId_;
	// Supress output and errors
	command <<  " > /dev/null 2> /dev/null";

	stringstream message;
	message << "Starting container: " << command.str();
	Log->info(message.str());

	Utils::myExec((command.str()).c_str());

	// So we should wait until it loads in cycle for maximum timeout() ms
	size_t counter = 0;
	const size_t step = getTimeStep();
	while (counter < getTimeout() && !isRunning_) {
		updateStatus();
		usleep(step * 1000);
		counter += step;
	}

	if(counter < getTimeout()) {
		Log->info("Container successfully started");
	}
	else {
		Log->error("Timeout reached while starting container");
	}

	return isRunning_;
}

bool Container::isExist() const 
{
	return isExist_;
}

bool Container::isRunning() const 
{
	return isRunning_;
}

void Container::updateStatus() 
{
	Logger *Log = Logger::getInstance();

	stringstream command;
	command << "vzctl status ";
	command << ctId_;

	stringstream message;
	message << "Checking status of container: " << command.str();
	Log->info(message.str());
	string result = Utils::myExec(command.str().c_str());
	
	message.str("");
	message << "Returned result: " << endl << result;
	Log->debug(message.str());
	
	if(result.length() > 0) {
		vector<string> params;
		Utils::split(result, ' ', params);
		
		if(params.size() == 5) {
			isExist_   = (strncmp(params[2].c_str(), "exist",   
						  strlen("exist"))   == 0);
			isRunning_ = (strncmp(params[4].c_str(), "running", 
						  strlen("running")) == 0);
			
			message.str("");
			message << "Container is exist: " << isExist_ << endl;
			message << "Container is running: " << isRunning_ << endl;
			Log->info(message.str());
		}
		else {
			Log->error("Vzctl returned string in unknown format.");
		}
	} 
	else {
		Log->error("Vzctl returned empty string.");
	}
}