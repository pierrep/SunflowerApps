//
// Log.cpp
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

#include "Log.h"

#include <stdarg.h>
#include <syslog.h>

//#define LOG_TO_SYSLOG

// Static member definitions
std::string * Log::myName = NULL;
LogLevel Log::myLogLevel = LOG_DEFAULT_LEVEL;
bool Log::myConsoleEnabled = false;

void Log::begin(const char * name)
{
    begin(name, LOG_DEFAULT_LEVEL);
}

void Log::begin(const char * name, int level)
{

    // Default to console output disabled
    myConsoleEnabled = false;

    // Set the name (cleaning up any previous references
    if (myName != NULL) {
        delete myName;
    }
    myName = new std::string(name);

    // Open syslog
#if defined(LOG_TO_SYSLOG)
    setlogmask(LOG_UPTO(level));
    openlog(myName->c_str(), LOG_OPTIONS, LOG_FACILITY);
#endif
}

void Log::end()
{
    if (myName != NULL) {
        delete myName;
    }
    myName = NULL;
#if defined(LOG_TO_SYSLOG)
    closelog();
#endif
}

void Log::debug(const char * format, ...)
{
    va_list args;
    va_start(args, format);

//#if defined(LOG_TO_SYSLOG)
//    vsyslog(LOG_DEBUG, format, args);
//#else
    if (myLogLevel < LogLevel::Debug) return;

    printf("DEBUG - ");
    vprintf(format, args);
    printf("\n");
//#endif

    va_end(args);
}

void Log::warn(const char * format, ...)
{
    va_list args;
    va_start(args, format);

#if defined(LOG_TO_SYSLOG)
    vsyslog(LOG_WARNING, format, args);
#else
    printf("WARN - ");
    vprintf(format, args);
    printf("\n");
#endif

    va_end(args);
}

void Log::error(const char * format, ...)
{
    va_list args;
    va_start(args, format);

#if defined(LOG_TO_SYSLOG)
    vsyslog(LOG_ERR, format, args);
#else
    printf("ERR - ");
    vprintf(format, args);
    printf("\n");
#endif

    va_end(args);
}

void Log::info(const char * format, ...)
{
    va_list args;
    va_start(args, format);

#if defined(LOG_TO_SYSLOG)
    vsyslog(LOG_INFO, format, args);
#else
    //printf("INFO - ");
    vprintf(format, args);
    printf("\n");
#endif

    va_end(args);
}

void Log::notice(const char * format, ...)
{
    va_list args;
    va_start(args, format);

#if defined(LOG_TO_SYSLOG)
    vsyslog(LOG_NOTICE, format, args);
#else
    printf("NOTICE - ");
    vprintf(format, args);
    printf("\n");
#endif

    va_end(args);
}

void Log::setConsoleEnabled(bool enabled)
{
    myConsoleEnabled = enabled;
}

bool Log::getConsoleEnabled(bool enabled)
{
    return myConsoleEnabled;
}

void Log::setLevel(LogLevel level)
{

    // Quick sanity check
    if (level < LOG_EMERG || level > LOG_DEBUG) {
        return;
    }

    myLogLevel = level;

#if defined(LOG_TO_SYSLOG)
    setlogmask((int)level);
#endif
}

LogLevel Log::getLevel()
{
    return myLogLevel;
}
