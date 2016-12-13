//
// HTTPEmitter.h
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


#ifndef __SunflowerDataService__HTTPEmitter__
#define __SunflowerDataService__HTTPEmitter__

#include <iostream>
#include <string>
#include <map>

#include "IEmitter.h"

using namespace std;

class HTTPEmitter : public IEmitter
{

public:
    virtual void emit();

    void addField(string name, string value);
    void setURL(string url);

private:
    string myURL;
    map<string, string> myFields;
};

#endif /* defined(__SunflowerDataService__HTTPEmitter__) */
