//
// OSCEmitter.cpp
//
// Copyright (c) 2016 Pierre Proske. All rights reserved.
//


#ifndef __SunflowerDataService__OSCEmitter__
#define __SunflowerDataService__OSCEmitter__

#define OUTPUT_BUFFER_SIZE 1024

#include <iostream>
#include <string>
#include <unordered_map>
#include "ip/UdpSocket.h"

#include "IEmitter.h"


using namespace std;

class OSCEmitter : public IEmitter
{

public:
    virtual void emit();

    OSCEmitter();

    void addData(unordered_map<string, string> &results);
    void setPort(int port);
    void setIP(string ip_address);

private:
    string myIP;
    int myPort;
    unordered_map<string, string> results;
    char buffer[OUTPUT_BUFFER_SIZE];

};

#endif

