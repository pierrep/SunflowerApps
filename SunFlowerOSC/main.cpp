//
// main.cpp
//
// This file is part of the SunflowerDataService application.
//
// SunflowerDataService is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SunflowerDataService is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SunflowerDataService.  If not, see <http://www.gnu.org/licenses/>.
//
// Created by Makina on behalf of Southern Cross University.
// Copyright (c) 2014 Makina. All rights reserved.
//


#include <iostream>
#include <map>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <vector>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "Config.h"
#include "Daemon.h"
#include "Log.h"

#include "HTTPEmitter.h"
#include "JSONFormatter.h"


using namespace std;

/*
 * Global application definitions
 */


#define APP_NAME        "SunflowerDataService"
#define APP_VERSION     "V01.04.00"
#define DEFAULT_PORT    "/dev/ttyUSB0"

Daemon * myDaemon = NULL;

namespace Option
{
/*
 * Option definitions
 */
static const char HELP          = 'h';
static const char VERSION       = 'v';
static const char DEBUG_LEVEL   = 'd';
static const char FOREGROUND    = 'f';
static const char CONFIG        = 'C';
static const char PORT          = 'p';
static const char INTERVAL      = 'i';
static const char UID           = 'u';

static const char * shortOptions = "hvdfC:p:i:u:";

static struct option longOptions[] = {

    { "help",       no_argument, 	NULL, HELP },
    { "version", 	no_argument, 	NULL, VERSION },
    { "debug", 		no_argument, 	NULL, DEBUG_LEVEL },
    { "foreground", no_argument, 	NULL, FOREGROUND },
    { "config",		required_argument,	NULL, CONFIG },
    { "port",		required_argument,	NULL, PORT },
    { "interval",	required_argument,	NULL, INTERVAL },
    { "uid",	    required_argument,	NULL, UID },

    { NULL, 0, NULL, 0 }

};

/*
 * Option values
 */
bool isDebugMode = false;
bool isForeground = false;
std::string * configPath = NULL;
std::string * serialPort = NULL;
int interval = 1;
int uid = 0;
}

void displayVersion()
{
    cout << "SunflowerDataService " << APP_VERSION << endl;
}

void displayUsage()
{
    displayVersion();

    cout << "Usage: SunflowerDataService [-hvdf] [-C PATH]" << endl << endl;
    cout << "   h : Show this usage description" << endl;
    cout << "   v : Show version information only" << endl;
    cout << "   d : Set the logging level to DEBUG" << endl;
    cout << "   f : Run as a foreground app (do not daemonise)" << endl;
    cout << "   p : Set the serial port" << endl;
    cout << "   i : Set the polling interval in seconds (defaults to 10)" << endl;
    cout << "   C : use PATH as an alternate configuration file." << endl;
    cout << "   u : Set the process user id." << endl << endl;
}

int readOptions(int argc, char * argv[])
{

    signed char ch;

    /*

     int	getopt_long(int, char * const *, const char *, const struct option *, int *);
     int	getopt_long_only(int, char * const *, const char *,
     const struct option *, int *);
    */

    // Restart option scanning
    while ((ch = getopt_long(argc, argv, Option::shortOptions, Option::longOptions, NULL)) != -1) {

        switch (ch) {

        case Option::HELP:
            displayUsage();
            exit(EXIT_SUCCESS);
            break;

        case Option::VERSION:
            displayVersion();
            exit(EXIT_SUCCESS);
            break;

        case Option::DEBUG_LEVEL:
            Log::debug("readOptions: Debug logging enabled");
            Log::setLevel(LogLevel::Debug);
            break;

        case Option::FOREGROUND:
            Log::setConsoleEnabled(true);
            Log::debug("readOptions: Foreground (console) mode enabled");
            break;

        case Option::CONFIG:
            Option::configPath = new std::string(optarg);
            Log::debug("readOptions: Setting configuration path to '%s'", Option::configPath->c_str());
            break;

        case Option::PORT:
            Option::serialPort = new std::string(optarg);
            Log::debug("readOptions: Setting serial port to '%s'", Option::serialPort->c_str());
            break;

        case Option::INTERVAL:
            Option::interval = atoi(optarg);
            Log::debug("readOptions: Setting polling interval to '%d'", Option::interval);
            break;

        case Option::UID:
            Option::uid = atoi(optarg);
            Log::debug("readOptions: Setting uid to '%d'", Option::uid);
            break;

        default:
            Log::debug("readOptions: Invalid command-line argument supplied %d.", ch);
            displayUsage();
            exit(EXIT_FAILURE);
            break;
        }

    }

    return 0;
}

int readConfiguration()
{
    return 0;
}


volatile bool isFinished;

void startDaemon()
{

    Log::info("Starting %s", APP_NAME);

    myDaemon = new Daemon();

    try {

        myDaemon->start(Option::serialPort, Option::interval);

        // Reset the isFinished flag
        isFinished = false;

    } catch (int exception) {
        delete myDaemon;
        myDaemon = NULL;
        Log::error("Failed to start the daemon process, aborting.");
        return;
    }

    // Perform work
    while (!isFinished) {

        myDaemon->work();

        Log::debug("Waiting for next poll (%d second)", Option::interval);
        sleep(Option::interval);
    }
}

void stopDaemon()
{
    // This may be called by a signal handler as well as through atexit(), so only run once.
    if (myDaemon != NULL) {
        Log::info("Shutting down %s", APP_NAME);

        myDaemon->stop();
        delete myDaemon;
        myDaemon = NULL;
    }
}

void signalHandler(int signo)
{
    if (signo == SIGINT) {
        Log::info("Caught SIGINT");
        isFinished = true;
        stopDaemon();
        exit(0);
    } else if (signo == SIGTERM) {
        Log::info("Caught SIGTERM");
        isFinished = true;
    }
}

int main(int argc, char * argv[])
{


    /*
     * Configuration
     */

    // Set the defaults
    Option::serialPort = new string(DEFAULT_PORT);

    // Read the command-line options
    readOptions(argc, argv);

    // Initialise the logging first
    Log::begin(APP_NAME);

    // Read the application configuration
    Log::debug("main: Reading configuration file");
    if (readConfiguration() != 0) {
        Log::error("Invalid configuration, exiting");
        exit(0);
    }


    /*
     * Daemon
     */

    /*
        // Check if we are already a daemon (if we have a parent process)
        if (getppid() == 1) return 0;

        // Fork the process
        pid_t pid = fork();

        // If failed, exit with an error
        if (pid < 0) exit(1);

        // If successfull, exit the parent process (the daemon process will continue)
        if (pid > 0) exit(0);

        // Obtain a new process group
        pid_t sid = setsid();
    */
    // Trap signals
    signal(SIGCHLD,SIG_IGN); /* ignore child */
    signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    /*
     * Application
     */

    // Set the application locale (default)
    Log::debug("main: Setting locale to LC_ALL");
    setlocale(LC_ALL, "");

    // Start the service daemon
    startDaemon();

    Log::info("Shutting down");

    stopDaemon();

    return 0;
}

