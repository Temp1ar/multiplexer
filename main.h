#ifndef MAIN_H
#define MAIN_H

#include <getopt.h>
#include <iostream>
#include <sstream>
#include <string.h>

#include "Logger.h"
#include "Container.h"
#include "VNCServer.h"
#include "ConfigFile.h"

const int BAD_CTID_RECEIVED = 1;
const int CANT_START_SERVER = 2;
const int NO_SUCH_CONTAINER = 3;

const char* DEFAULT_CONFIG = "/etc/mplex.conf";
const int BAD_CONFIG_FILE = 5;

const int BAD_ARGUMENTS = 6;

#endif // MAIN_H