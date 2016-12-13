This file is part of the SunflowerDataService application.

SunflowerDataService is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SunflowerDataService is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
  along with SunflowerDataService.  If not, see <http://www.gnu.org/licenses/>.


HOW TO COMPILE:

1) Dependancies

You need the following dependancies to compile the SunflowerDataService:

JSON - Boost (property_tree)
http://www.boost.org/doc/libs/1_41_0/doc/html/boost_propertytree/tutorial.html
(On Ubuntu/Raspbian, 'sudo apt-get install libboost-all-dev')

CONFIG - Boost (property_tree)
http://www.boost.org/doc/libs/1_41_0/doc/html/boost_propertytree/tutorial.html
(On Ubuntu/Raspbian, 'sudo apt-get install libboost-all-dev')

HTTP POST - LIBCURL
http://curl.haxx.se/libcurl/c/example.html
(On Ubuntu/Raspbian, 'sudo apt-get install libcurl-dev')

2) Generate a CMake file

This project utilises CMake (https://cmake.org), A widely-used cross-platform build generator. This will let you produce a build filesystem for most popular C++ development environments on most platforms.

On linux, to compile, simple perform the following:

1) Enter the <project>\build directory (or create it if it does not exit)

2) Type 'cmake ..' to generate the Makefile

3) Type 'make' to compile


APPLICATION OPTIONS:

Usage: SunflowerDataService [-hvdpi]

   h : Show this usage description
   v : Show version information only
   d : Set the logging level to DEBUG
   p : Set the serial port
   i : Set the polling interval in milliseconds (defaults to 1000)
