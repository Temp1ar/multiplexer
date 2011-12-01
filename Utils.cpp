#include "Utils.h" 

namespace Utils
{
	const int getBufferSize()
	{
		return 255;
	}

	std::string myExec(const char* command)
	{	
		FILE* pipe = popen(command, "r");
		if (pipe == 0) {
			Logger::getInstance()->error("Can't perform popen() operation.");
			exit(CANT_OPEN_PIPE);
		}
		

		char buffer[getBufferSize()];
		std::string result = "";
		while(!feof(pipe)) {
			if(fgets(buffer, getBufferSize(), pipe) != NULL)
					result += buffer;
		}

		pclose(pipe);
		return result;
	}

	std::vector<std::string>& split(const std::string& input, 
									const char delimiter, 
									std::vector<std::string>& elements) {
		std::stringstream ss(input);
		std::string item;
		while(getline(ss, item, delimiter)) {
			elements.push_back(item);
		}
		return elements;
	}
}