#include "SolarData.h"

SolarData::SolarData()
{
    maxLoad = 100;
    maxCharge = 100;
}

void SolarData::setup()
{
    // listen on the given port
    ofLogNotice() << "listening for osc messages on port " << PORT;
    receiver.setup(PORT);
}

void SolarData::update()
{

    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);

        if(m.getAddress() == "/BatteryVoltage"){
            batteryVoltage = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/BatteryPercent"){
            batteryPercent = m.getArgAsFloat(0);
        }
        else if(m.getAddress() == "/LoadW"){
            load = m.getArgAsFloat(0);

        }
        else if(m.getAddress() == "/InstantLoadW"){
            instantLoad = m.getArgAsFloat(0);
            if(instantLoad > maxLoad) {
                maxLoad = instantLoad;
            }
        }
        else if(m.getAddress() == "/ChargeW"){
            charge = m.getArgAsFloat(0);


        }
        else if(m.getAddress() == "/InstantChargeW"){
            instantCharge = m.getArgAsFloat(0);
            if(instantCharge > maxCharge) {
                maxCharge = instantCharge;
            }
        }

    }

}

float SolarData::getNormalisedLoad()
{

    float value = instantLoad/maxLoad;
    return value;

}

float SolarData::getNormalisedCharge()
{
    float value = instantCharge/maxCharge;
    return value;

}
