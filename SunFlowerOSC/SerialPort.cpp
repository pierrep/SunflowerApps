//
// SerialPort.cpp
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


#include "SerialPort.h"
#include "Log.h"

#include <stdio.h>     // Standard input/output definitions
#include <string.h>    // String function definitions
#include <unistd.h>   // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>     // Error number definitions
#include <termios.h>  // POSIX terminal control definitions


#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

void SerialPort::open(const char * port)
{

    if (isConnected()) {
        Log::error("SerialPort::write: Port is already open, close first");
        throw ERR_SERIALPORT_ISOPEN;
    }


    //open the port and store the file descriptor in 'port'
    if ((myHandle = ::open(port, O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0) {

        // Reset state
        myHandle = 0;

        // Could not open the port
        Log::error("SerialPort::open: Failed to open port %s", port);
        throw ERR_SERIALPORT_OPEN;
    }

    if (!isatty(myHandle)) {
        Log::error("SerialPort::open: Requested port is not a TTY (serial) device");
        ::close(myHandle);
        myHandle = 0;
        throw ERR_SERIALPORT_NOTAPORT;
    }

    // Set the port I/O options (return if no data available)
    //fcntl(myHandle, F_SETFL, FNDELAY);
    //int r = fcntl(myHandle, F_SETFL, FNDELAY);
    //Log::debug("SerialPort::open: fcntl returned %x", r);

    // Configure the port
    struct termios options;

    if (tcgetattr(myHandle, &options) != 0) {
        Log::error("SerialPort::open: Failed to read port options.");
        ::close(myHandle);
        myHandle = 0;
        throw ERR_SERIALPORT_CONFIG;
    }

    // Copy the options to our saved options to restore later
    memcpy(&mySavedOptions, &options, sizeof(termios));

//    cfmakeraw(&options);

    options.c_iflag &= ~IGNBRK;

    // Disable all local processing
    options.c_lflag = 0;

    // Data bits = 8
    options.c_cflag &= ~CSIZE; // Clear size mask
    options.c_cflag |= CS8; // Set

    // Parity = None
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~PARODD;

    // Stop Bits = 1
    options.c_cflag &= ~CSTOPB;

    // Don't canonicalise data
    options.c_cflag &= ~ICANON;

    // Don't echo
    options.c_cflag &= ~ECHO;

    // Disable XON/XOFF
    options.c_cflag &= ~IXON;
    options.c_cflag &= ~IXOFF;
    options.c_cflag &= ~IXANY;

    // Disable CTS/RTS
    options.c_cflag &= ~CRTSCTS;

    // Speed = 9600
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    // Disallow line status change
    options.c_cflag |= CLOCAL;

    // Enable receiver
    options.c_cflag |= CREAD;


    // Minimum # of bytes to read
    options.c_cc[VMIN] = 0;

    // Amount of time to wait in deciseconds
    options.c_cc[VTIME] = 4;

    /*
        cout << "c_iflag = " << hex << options.c_iflag << endl;
        cout << "c_oflag = " << hex << options.c_oflag << endl;
        cout << "c_lflag = " << hex << options.c_lflag << endl;
        cout << "c_cflag = " << hex << options.c_cflag << endl;
    */

    // Flush the port and apply the changes
    if (tcsetattr(myHandle, TCSANOW, &options) != 0) {
        Log::error("SerialPort::open: Failed to configure port options.");
        ::close(myHandle);
        myHandle = 0;
        throw ERR_SERIALPORT_CONFIG;
    }

    tcflush(myHandle, TCIFLUSH);
}

void SerialPort::close()
{

    if (!isConnected()) {
        Log::error("SerialPort::close: Port is not open");
        throw ERR_SERIALPORT_ISCLOSED;
    }

    // Restore the saved terminal options
    if (tcsetattr(myHandle, TCSANOW, &mySavedOptions) != 0) {
        Log::error("SerialPort::open: Failed to configure port options.");
    }


    ::close(myHandle);
    myHandle = 0;
}

bool SerialPort::isConnected()
{

    return (myHandle != 0);

}

void SerialPort::write(const unsigned char * buffer, int length)
{

    if (!isConnected()) {
        Log::error("SerialPort::write: Port is not open");
        throw ERR_SERIALPORT_ISCLOSED;
    }

    tcflush(myHandle, TCIOFLUSH);
    std::stringstream ss;
    ss << "SerialPort::write: ";
    for (int i = 0; i < length; i++)
    {
        ss << setfill('0') << hex << setw(2) << static_cast<unsigned>(buffer[i]) << " ";
    }
    ss << "(length " << length << ")";
    Log::debug(ss.str().c_str());

    ssize_t len = ::write(myHandle, buffer, length);
    usleep(50 * 1000);
    if (len < 0) {
        Log::error("SerialPort::write: Failed to write data");
        throw ERR_SERIALPORT_WRITE;
    }

    tcdrain(myHandle);
}

void SerialPort::write(unsigned char value)
{

    if (!isConnected()) {
        Log::error("SerialPort::write: Port is not open");
        throw ERR_SERIALPORT_ISCLOSED;
    }

    tcflush(myHandle, TCIOFLUSH);

    ssize_t len = ::write(myHandle, &value, 1);
    if (len < 0) {
        Log::error("SerialPort::write: Failed to write data");
        throw ERR_SERIALPORT_WRITE;
    }


}


int SerialPort::read(unsigned char * buffer, int length)
{

    if (!isConnected()) {
        Log::error("SerialPort::read: Port is not open");
        throw ERR_SERIALPORT_ISCLOSED;
    }

    ssize_t len = 0;
    memset(buffer, 0, length);
    while ((len = ::read(myHandle, buffer, length)) < 0) {
        // Sleep 10ms
        usleep(50 * 1000);
    }

    if (len < 0) {
        Log::error("SerialPort::read: Failed to read data");
        throw ERR_SERIALPORT_READ;
    }

    std::stringstream ss;
    ss << "SerialPort::read: ";
    for (int i = 0; i < len; i++) {
        ss << setfill('0') << hex << setw(2) << static_cast<unsigned>(buffer[i]) << " ";
    }
    ss << "(length " << len << ")";
    Log::debug(ss.str().c_str());

    return (int)len;
}

void SerialPort::sendBreak(int duration)
{
    if (!isConnected()) {
        Log::error("SerialPort::read: Port is not open");
        throw ERR_SERIALPORT_ISCLOSED;
    }

    /*
     * NOTE: In OSX, the duration is ignored and defaults to 400ms
     */

    tcsendbreak(myHandle, duration);
}
