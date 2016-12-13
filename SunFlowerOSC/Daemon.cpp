//
// Daemon.cpp
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

#include "Daemon.h"
#include "Log.h"
#include "PL40.h"
#include "JSONFormatter.h"
#include "HTTPEmitter.h"
#include "OSCEmitter.h"

#include <sys/stat.h>
#include <time.h>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>


using namespace std;

void Daemon::start(string * serialPort, int interval)
{

    if(USE_PL40) {
        // Connect to the PL40
        Log::debug("Daemon::start: serialPort=%s, interval=%d", serialPort->c_str(), interval);
        mySerialPort = serialPort;
        myPL40 = new PL40(serialPort->c_str());
        myPL40->connect();
    }
    else {
        reader.open("/home/pi/code/SunFlowerOSC/basic_data_short.csv");
    }

    // Start the daemon
    Log::notice("Sunflower logging service started");

    myInterval = interval;
}

void Daemon::stop()
{

    // Stop the daemon
    Log::notice("Sunflower logging service stopped");

    myPL40->disconnect();
    delete myPL40;
}

string getDateTimeString()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%Y-%m-%dT%T.000Z", timeinfo);
    return buffer;
}

int getHourCounter() {

    time_t sec = time (NULL);

    return (sec / 3600);
}

int lastHourCounter = 0;

void Daemon::work()
{
    JSONFormatter json;

    cout << " do Work..." << endl;

    // Read the requested PL40 parameters
    unordered_map<string, string> basicData;

    if(USE_PL40) {
        myPL40->poll(&basicData);
    } else {

        vector<string> output = reader.getNextLineAndSplitIntoTokens();

        typedef pair<string, string> KV;

        if(output.size() == 7) {
            // BatteryPercent (limit to 100)
            basicData.insert(KV("BatteryPercent", output[2]));

            // LoadW
            basicData.insert(KV("LoadW", output[1]));

            // InstantLoadW
            basicData.insert(KV("InstantLoadW", output[3]));

            // InstantChargeW
            basicData.insert(KV("InstantChargeW", output[4]));

            // ChargeW
            basicData.insert(KV("ChargeW", output[5]));

            // BatteryVoltage
            basicData.insert(KV("BatteryVoltage", output[6]));
        }
        /*
        typedef pair<string, string> KV;

        // BatteryPercent (limit to 100)
        basicData.insert(KV("BatteryPercent", to_string(100)));

        // LoadW
        basicData.insert(KV("LoadW", to_string((int)300)));

        // InstantLoadW
        basicData.insert(KV("InstantLoadW", to_string((int)600)));

        // InstantChargeW
        basicData.insert(KV("InstantChargeW", to_string((int)400)));

        // ChargeW
        basicData.insert(KV("ChargeW", to_string((int)240)));

        // BatteryVoltage
        basicData.insert(KV("BatteryVoltage", to_string(28.8)));
        */
    }


    // Read the local date/time
    string dateTime = getDateTimeString();

    // Set the date/time
    json.addDataPointString("UpdateDateTime", dateTime);

    // Add the data elements returned from the PL40
    BOOST_FOREACH(auto& f, basicData) {
        json.addDataPoint(f.first, f.second);
    }
    Log::debug(json.render().c_str());

    // Render the data out to the HTTP server file
    try
    {
        ofstream ofile;
        ofile.open("/var/www/data/basic_data.json");
        ofile << json.render();
        ofile.close();
    }
    catch(exception& ex) {
        Log::debug("ERROR Writing basic_data.json");
        cout << "EXCEPTION MESSAGE: " << ex.what() << endl;
    }

    // Render the data out to the HTTP server RAW file
    {
        ofstream ofile;
        ofile.open("/var/www/data/basic_data.csv", ofstream::out | ofstream::app);

        ofile << dateTime;

        BOOST_FOREACH(auto& f, basicData) {
            ofile << "," << f.second;
        }


        ofile << endl;
        ofile.close();
    }

    bool readHistory = true;

    if(USE_PL40) {
        if ( (lastHourCounter < getHourCounter()) && readHistory ) {

            // Read the requested PL40 history
            PL40History record[30];

            unordered_map<int, PL40History> historyData;

            int histCount = 15;

            for (int i = 0; i < histCount; i++) {
                try
                {
                    myPL40->readHistory(i + 1, &record[i]);
                }
                catch (...)
                {
                    Log::error("Error reading history record %i\n", i);
                }
            }

            for (int i = histCount-1; i >= 0; i--) {
                historyData[i+1] = record[i];
            }

            // Generate the JSON message
            {
                ofstream ofile;
                ofile.open("/var/www/data/history.json");
                ofile << json.renderHistory(historyData);
                ofile.close();
            }
            // Update the last hour counter
            lastHourCounter = getHourCounter();
        }
    } // if USE_PL40

    // Post the HTTP message (requires a web server url that will accept json)
    /*
         HTTPEmitter http;
         http.setURL("http://127.0.0.1/posts");
         http.addField("json", json.render());
         Log::info("Sending HTTP Post data");
         http.emit();
    */

    // Post the HTTP message (requires a web server url that will accept json)

         OSCEmitter osc;
         osc.setIP("127.0.0.1");
         osc.setPort(12345);
         osc.addData(basicData);
         Log::info("Sending OSC data");
         osc.emit();

}
