//
// Logger.h
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


#ifndef __SunflowerDataService__Logger__
#define __SunflowerDataService__Logger__

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#define LOG_OPTIONS         LOG_CONS | LOG_NDELAY | LOG_USER
#define LOG_DEFAULT_LEVEL   LogLevel::Debug
#define LOG_FACILITY        LOG_LOCAL1

enum LogLevel {

    Emergency   = LOG_EMERG,
    Alert       = LOG_ALERT,
    Critical    = LOG_CRIT,
    Error       = LOG_ERR,
    Warning     = LOG_WARNING,
    Notice      = LOG_NOTICE,
    Info        = LOG_INFO,
    Debug       = LOG_DEBUG

};

#define	LOG_EMERG	0	/* system is unusable */
#define	LOG_ALERT	1	/* action must be taken immediately */
#define	LOG_CRIT	2	/* critical conditions */
#define	LOG_ERR		3	/* error conditions */
#define	LOG_WARNING	4	/* warning conditions */
#define	LOG_NOTICE	5	/* normal but significant condition */
#define	LOG_INFO	6	/* informational */
#define	LOG_DEBUG	7	/* debug-level messages */

class Log
{

public:
    static void begin(const char * name);
    static void begin(const char * name, int level);
    static void end();

    static void debug(const char * fmt, ...);
    static void warn(const char * fmt, ...);
    static void error(const char * fmt, ...);
    static void info(const char * fmt, ...);
    static void notice(const char * fmt, ...);

    static void setConsoleEnabled(bool enabled);
    static bool getConsoleEnabled(bool enabled);

    static void setLevel(LogLevel level);
    static LogLevel getLevel();

private:
    static LogLevel myLogLevel;
    static std::string * myName;
    static bool myConsoleEnabled;
};



#endif /* defined(__SunflowerDataService__Logger__) */
