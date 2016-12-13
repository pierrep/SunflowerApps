//
// OSCEmitter.cpp
//
// Copyright (c) 2016 Pierre Proske. All rights reserved.
//

#include "OSCEmitter.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include "Log.h"

#include <boost/foreach.hpp>
#include <cstring>
#include <string>

using namespace std;

OSCEmitter::OSCEmitter()
{
    myPort = 12345;
    myIP = "127.0.0.1";
}

void OSCEmitter::setPort(int _port)
{
    myPort = _port;
}

void OSCEmitter::setIP(string _ip)
{
    myIP = _ip;
}


void OSCEmitter::addData(unordered_map<string, string> & _results)
{
    results = _results;
}

void OSCEmitter::emit()
{
    UdpTransmitSocket transmitSocket( IpEndpointName( myIP.c_str(), myPort ) );

    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

    p << osc::BeginBundleImmediate;


    BOOST_FOREACH(auto& r, results) {
       string s = "/"+r.first;
       p  << osc::BeginMessage(s.c_str());
       p << stof(r.second);
       p << osc::EndMessage;
    }

    p << osc::EndBundle;


    transmitSocket.Send( p.Data(), p.Size() );






}
