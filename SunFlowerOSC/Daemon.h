//
// Daemon.h
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


#ifndef __SunflowerDataService__Daemon__
#define __SunflowerDataService__Daemon__

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PL40.h"
#include "JSONFormatter.h"
#include "HTTPEmitter.h"
#include "CSVReader.h"

#define USE_PL40 1

class Daemon
{

public:

    void start(string * serialPort, int interval);
    void stop();
    void work();

private:
    PL40 * myPL40;
    string * mySerialPort;
    int myInterval;
    CSVReader reader;
};


#endif /* defined(__SunflowerDataService__Daemon__) */
