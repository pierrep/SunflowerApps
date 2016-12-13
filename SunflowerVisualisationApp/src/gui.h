#ifndef GUI_H
#define GUI_H

#include "ofMain.h"
#include "ofxGui.h"

class Gui
{
public:
    Gui();
    ~Gui();

    void setup();
    void update();
    void draw();

    void vSyncChanged(bool & vSync);


    ofXml settings;
    ofxPanel gui;
    ofParameter<bool> vSync;
    ofParameter<ofColor> loadColour;
    ofParameter<ofColor> chargeColour;
    ofParameter<ofColor> batteryColour;
    ofParameter<float> xpos;
    ofParameter<float> ypos;
    ofParameter<string> fps;
    //ofxLabel fps;

    ofParameterGroup parameters;
};

#endif // GUI_H
