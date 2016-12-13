//
// JSONFormatter.cpp
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

#include "JSONFormatter.h"


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <set>
#include <string>

using namespace std;
using boost::property_tree::ptree;

void JSONFormatter::setRoot(string name)
{
    myRoot = name;
}

void JSONFormatter::setTimestamp(string timeStamp)
{
    myTimestamp = timeStamp;
}

void JSONFormatter::addDataPoint(string name, string value)
{
    myDataPoints[name] = value;
}

void JSONFormatter::addDataPointString(string name, string value)
{
    stringstream ss;
    ss << "\"" << value << "\"";
    myDataPoints[name] = ss.str();
}

string JSONFormatter::renderHistory(unordered_map<int, PL40History> history)
{
    stringstream ss;
    int count;

    ss << "{ " << endl;

    // Categories
    ss << "\"categories\": [";

    ss << "\"Yesterday\",";
    ss << "\"-2\",";
    ss << "\"-3\",";
    ss << "\"-4\",";
    ss << "\"-5\",";
    ss << "\"-6\",";
    ss << "\"-7\",";
    ss << "\"-8\",";
    ss << "\"-9\",";
    ss << "\"-10\",";
    ss << "\"-11\",";
    ss << "\"-12\",";
    ss << "\"-13\",";
    ss << "\"-14\",";
    ss << "\"-15\"";

    ss << "], " << endl;

    // VMAX
    ss << "\"vmax\": [";

    count = 0;
    BOOST_FOREACH(auto& p, history) {

        ss << p.second.Vmax;
        if (count++ != (history.size() -1)) ss << ", ";
    }

    ss << "], " << endl;

    // VMIN
    ss << "\"vmin\": [";

    count = 0;
    BOOST_FOREACH(auto& p, history) {

        ss << p.second.Vmin;
        if (count++ != (history.size() -1)) ss << ", ";
    }

    ss << "], " << endl;

    // AHout
    ss << "\"loadW\": [";

    count = 0;
    BOOST_FOREACH(auto& p, history) {

        ss << (int)(p.second.AHout) * 24;
        if (count++ != (history.size() -1)) ss << ", ";
    }

    ss << "], " << endl;

    // AHin
    ss << "\"chargeW\": [";

    count = 0;
    BOOST_FOREACH(auto& p, history) {

        ss << (int)(p.second.AHin) * 24;
        if (count++ != (history.size() -1)) ss << ", ";
    }

    ss << "]" << endl;
    /*
        // SOC
        ss << "\"SOC\": [";

        count = 0;
        BOOST_FOREACH(auto& p, history) {

            ss << p.second.SOC;
            if (count++ != (history.size() -1)) ss << ", ";
        }

        ss << "], " << endl;
    */
    ss << "}";


    return ss.str();
}

string renderHistory9(unordered_map<int, PL40History> history)
{
    // Write the structure out to the stream
    stringstream ss;

    // Opening brace
    ss << "{ " << endl;

    ss << "\"labels\": [" << endl;
    ss << "\"1\"," << endl;
    ss << "\"2\"," << endl;
    ss << "\"3\"," << endl;
    ss << "\"4\"," << endl;
    ss << "\"5\"," << endl;
    ss << "\"6\"," << endl;
    ss << "\"7\"," << endl;
    ss << "\"8\"," << endl;
    ss << "\"9\"," << endl;
    ss << "\"10\"," << endl;
    ss << "\"11\"," << endl;
    ss << "\"12\"," << endl;
    ss << "\"13\"," << endl;
    ss << "\"14\"," << endl;
    ss << "\"15\"" << endl;
    ss << "], " << endl;

    ss << "\"datasets\": [" << endl;

// VMAX DATASET
    ss << "{ " << endl;

    ss << "\"label\": \"VMax\"," << endl;
    ss << "\"fillColor\": \"rgba(220,220,220,0.2)\"," << endl;
    ss << "\"strokeColor\": \"rgba(220,220,220,1)\"," << endl;
    ss << "\"pointColor\": \"rgba(220,220,220,1)\"," << endl;
    ss << "\"pointColorStroke\": \"#fff\"," << endl;
    ss << "\"pointHighlightFill\": \"#fff\"," << endl;
    ss << "\"pointHighlightStroke\": \"rgba(220,220,220,1)\"," << endl;

    ss << "\"data\": [" << endl;

    int count = 0;
    BOOST_FOREACH(auto& p, history) {

        if (count == (history.size() - 1)) {
            ss << p.second.Vmax  << endl;

        } else {
            ss << p.second.Vmax << ", " << endl;

        }
        count++;
    }



    ss << "] " << endl;
    ss << "}, " << endl;

// END VMAX DATA SET

// VMIN DATASET
    ss << "{ " << endl;

    ss << "\"label\": \"VMin\"," << endl;
    ss << "\"fillColor\": \"rgba(220,220,220,0.2)\"," << endl;
    ss << "\"strokeColor\": \"rgba(220,220,220,1)\"," << endl;
    ss << "\"pointColor\": \"rgba(220,220,220,1)\"," << endl;
    ss << "\"pointColorStroke\": \"#fff\"," << endl;
    ss << "\"pointHighlightFill\": \"#fff\"," << endl;
    ss << "\"pointHighlightStroke\": \"rgba(220,220,220,1)\"," << endl;

    ss << "\"data\": [" << endl;

    count = 0;;
    BOOST_FOREACH(auto& p, history) {

        if (count == (history.size() - 1)) {
            ss << p.second.Vmin  << endl;

        } else {
            ss << p.second.Vmin << ", " << endl;

        }
        count++;
    }

    ss << "] " << endl;
    ss << "} " << endl;
    ss << "] " << endl;
    ss << "} " << endl;
// END VMIN DATA SET

    // Data Elements

    // Closing brace

    return ss.str();
}
string JSONFormatter::renderHistory2(unordered_map<int, PL40History> history)
{
    // Write the structure out to the stream
    stringstream ss;

    // Opening brace
    ss << "{ " << endl;

    ss << "\"labels\": [" << endl;
    ss << "\"1\"," << endl;
    ss << "\"2\"," << endl;
    ss << "\"3\"," << endl;
    ss << "\"4\"," << endl;
    ss << "\"5\"," << endl;
    ss << "\"6\"," << endl;
    ss << "\"7\"," << endl;
    ss << "\"8\"," << endl;
    ss << "\"9\"," << endl;
    ss << "\"10\"," << endl;
    ss << "\"11\"," << endl;
    ss << "\"12\"," << endl;
    ss << "\"13\"," << endl;
    ss << "\"14\"," << endl;
    ss << "\"15\"" << endl;
    /*
    ss << "\"16\"," << endl;
    ss << "\"17\"," << endl;
    ss << "\"18\"," << endl;
    ss << "\"19\"," << endl;
    ss << "\"20\"," << endl;
    ss << "\"21\"," << endl;
    ss << "\"22\"," << endl;
    ss << "\"23\"," << endl;
    ss << "\"24\"," << endl;
    ss << "\"25\"," << endl;
    ss << "\"26\"," << endl;
    ss << "\"27\"," << endl;
    ss << "\"28\"," << endl;
    ss << "\"29\"," << endl;
    ss << "\"30\"" << endl;
    */
    ss << "], " << endl;

    ss << "\"datasets\": [" << endl;

// VMAX DATASET
    ss << "{ " << endl;

    ss << "\"label\": \"AHout\"," << endl;
    ss << "\"fillColor\": \"rgba(220,220,220,0.2)\"," << endl;
    ss << "\"strokeColor\": \"rgba(220,220,220,1)\"," << endl;
    ss << "\"pointColor\": \"rgba(220,220,220,1)\"," << endl;
    ss << "\"pointColorStroke\": \"#fff\"," << endl;
    ss << "\"pointHighlightFill\": \"#fff\"," << endl;
    ss << "\"pointHighlightStroke\": \"rgba(220,220,220,1)\"," << endl;

    ss << "\"data\": [" << endl;

    int count = 0;
    BOOST_FOREACH(auto& p, history) {

        if (count == (history.size() - 1)) {
            ss << p.second.AHout  << endl;

        } else {
            ss << p.second.AHout << ", " << endl;

        }
        count++;
    }



    ss << "] " << endl;
    ss << "}, " << endl;

// END VMAX DATA SET

// VMIN DATASET
    ss << "{ " << endl;

    ss << "\"label\": \"AHin\"," << endl;
    ss << "\"fillColor\": \"rgba(220,220,220,0.2)\"," << endl;
    ss << "\"strokeColor\": \"rgba(220,220,220,1)\"," << endl;
    ss << "\"pointColor\": \"rgba(220,220,220,1)\"," << endl;
    ss << "\"pointColorStroke\": \"#fff\"," << endl;
    ss << "\"pointHighlightFill\": \"#fff\"," << endl;
    ss << "\"pointHighlightStroke\": \"rgba(220,220,220,1)\"," << endl;

    ss << "\"data\": [" << endl;

    count = 0;;
    BOOST_FOREACH(auto& p, history) {

        if (count == (history.size() - 1)) {
            ss << p.second.AHin  << endl;

        } else {
            ss << p.second.AHin << ", " << endl;

        }
        count++;
    }

    ss << "] " << endl;
    ss << "} " << endl;
    ss << "] " << endl;
    ss << "} " << endl;
// END VMIN DATA SET

    // Data Elements

    // Closing brace

    return ss.str();
}


string JSONFormatter::render()
{
    // Build the data points array

    // Write the structure out to the stream
    stringstream ss;

    // Opening brace
    ss << "{ " << endl;

    // Data Elements
    BOOST_FOREACH(auto& p, myDataPoints) {
        if (p.first.compare(myDataPoints.rbegin()->first) == 0) {
            ss << "     \"" << p.first << "\": " << p.second << endl;
        }
        else {
            ss << "     \"" << p.first << "\": " << p.second << "," << endl;
        }
    }


    // Closing brace
    ss << "}";

    return ss.str();
}

