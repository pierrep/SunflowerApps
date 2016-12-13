#ifndef SOLARDATA_H
#define SOLARDATA_H

#define PORT 12345

#include "ofxOsc.h"

class SolarData
{
public:
    SolarData();
    void setup();
    void update();


    float getNormalisedCharge();
    float getNormalisedLoad();

    float batteryVoltage;
    float instantLoad;
    float load;
    float instantCharge;
    float charge;
    float batteryPercent;
    float maxLoad;
    float maxCharge;

protected:


    ofxOscReceiver receiver;
};

#endif // SOLARDATA_H
