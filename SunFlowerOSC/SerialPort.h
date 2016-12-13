//
// SerialPort.h
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


#ifndef __SunflowerDataService__SerialPort__
#define __SunflowerDataService__SerialPort__

#include <iostream>

#include <stdio.h>     // Standard input/output definitions
#include <string.h>    // String function definitions
#include <unistd.h>   // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>     // Error number definitions
#include <termios.h>  // POSIX terminal control definitions


class SerialPort
{

public:
    void open(const char * port);
    void close();
    bool isConnected();

    int read(unsigned char * buffer, int length);
    void write(const unsigned char * buffer, int length);
    void write(unsigned char byte);

    void sendBreak(int duration);

private:
    int myHandle;
    termios mySavedOptions;
};

//
// ERROR DEFINITIONS
//

#define ERR_SERIALPORT_ISOPEN       0x8001 // Serial port is already open
#define ERR_SERIALPORT_ISCLOSED     0x8002 // Serial port is already closed
#define ERR_SERIALPORT_OPEN         0x8003 // Serial port failed to open
#define ERR_SERIALPORT_CONFIG       0x8004 // Requested port is not a TTY (serial) port
#define ERR_SERIALPORT_NOTAPORT     0x8005 // Serial port failed to be configured
#define ERR_SERIALPORT_WRITE        0x8006 // Serial port failed to write
#define ERR_SERIALPORT_READ         0x8007 // Serial port failed to read

#endif /* defined(__SunflowerDataService__SerialPort__) */
