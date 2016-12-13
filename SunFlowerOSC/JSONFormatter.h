//
// JSONEmitter.h
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


#ifndef __SunflowerDataService__JSONEmitter__
#define __SunflowerDataService__JSONEmitter__

#include <iostream>
#include <string>
#include <map>
#include <set>

#include "PL40.h"

using namespace std;

typedef std::map<std::string, std::string> TKeyPairList;
typedef std::pair<std::string, std::string> TKeyPair;

class JSONFormatter
{

public:
    void setRoot(string name);
    void setTimestamp(string timeStamp);
    void addDataPoint(string name, string value);
    void addDataPointString(string name, string value);

    string render();
    string renderHistory(unordered_map<int, PL40History> history);
    string renderHistory2(unordered_map<int, PL40History> history);
private:

    string myRoot;
    string myTimestamp;
    map<string, string> myDataPoints;
    //    set<pair<string, string>> myDataPoints2;
};


#endif /* defined(__SunflowerDataService__JSONEmitter__) */


/*

void Test() {

    TKeyPairList list;

    //list.insert(TKeyPair("A", "Kim"));
    //list.insert(TKeyPair("B", "Jala"));
    //list.insert(TKeyPair("C", "Robyn"));

    list["A"] = "Kim";
    list["B"] = "Jala";
    list["C"] = "Robyn";

    TKeyPairList::iterator p;
    for (p = list.begin(); p != list.end(); p++) {

        cout << p->first << " is " << p->second << "." << endl;

        std::string key, value;
    }

    list["B"] = "Homer";

    for (p = list.begin(); p != list.end(); p++) {

        cout << p->first << " is " << p->second << "." << endl;

        std::string key, value;
    }
}
*/
/*
{
    "PL40":{
        "updateDateTime":"2014-04-01T09:01:22.56110+10:00",
        "updateCounter":"123",
        "data":[
                {
                    "name":"bmax",
                    "value":"29.9"
                },
                {
                    "name":"bmin",
                    "value":"24.9"
                },
                {
                    "name":"ltemp",
                    "value":"55"
                },
                {
                    "name":"dtime",
                    "value":"104"
                }
                ]
    }
}
*/
