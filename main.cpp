#include "main.h"

using namespace std;

int main(int argc, char *argv[]) 
{	
	stringstream message;
	Logger *Log = Logger::getInstance();
	string configPath(DEFAULT_CONFIG); 
	
	// Parsing arguments
	extern char *optarg;
	extern int optopt, optind, opterr;
    opterr = 0;
	int c;
	while ((c = getopt(argc, argv, "c:")) != -1) {
		message.str("");
		switch (c) {
			case 'c' :
				configPath = string(optarg);
				break;
			case '?' :
				if (optopt == 'c') {
					message << "Option -" << (char) optopt 
						 << " requires an argument.\n";
				}
				else if (isprint (optopt)) {
					message << "Unknown option `-" 
						 << (char) optopt << "'.\n";
				} else {
					message << "Unknown option character `\\x" 
							<< (char) optopt << "'\n";
				}
				Log->error(message.str());
				exit(BAD_ARGUMENTS);
			default :
				abort ();
		}
	}
        
	int index;
	for (index = optind; index < argc; index++) {
		message.str("");
		message << "Non-option argument " << argv[index] << "\n";
		Log->warning(message.str());
	}

	// Loading configuration file
	try {
		ConfigFile::filename = configPath;
		ConfigFile::getInstance();
	} catch(ConfigFile::file_not_found& e) {
		Log->error("Config file doesnt exist: " + e.filename);
		exit(BAD_CONFIG_FILE);
	}

	// Start talking with client
	size_t ctId;
	cin >> ctId;
	if(cin.fail()) {
		Log->error("Not numeric CTID received.");
		exit(BAD_CTID_RECEIVED);
	}

	message.str("");
	message << "Received container ID: " << ctId;
	Log->info(message.str());

	Container container(ctId);
	
	if (!container.isExist()) {
		message.str("");
		message << "No container with such ID: " << ctId;
		Log->error(message.str());
		exit(NO_SUCH_CONTAINER);
	}

	if (!container.isRunning()) {
		Log->info("Container exists, but is down. Starting...");
		container.run();
	}
	
	Log->info("Container is ready for work.");

	VNCServer server(ctId);

	if (!server.isRunning()) {		
		if (!server.run()) {
			Log->error("Can't create server instance.");
			cout << 0;
			exit(CANT_START_SERVER);
		}
	}

	message.str("");
	message << "Returning port to client: " << server.getPort();
	Log->info(message.str());
	cout << server.getPort();
	Log->info("Successfully terminated");

	return 0;
}
