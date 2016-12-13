#include "gui.h"

Gui::Gui()
{

}

Gui::~Gui()
{
    xpos.set(gui.getPosition().x);
    ypos.set(gui.getPosition().y);
    settings.serialize(parameters);
    settings.save("settings.xml");
}

void Gui::setup()
{
    parameters.setName("Settings");
    parameters.add(loadColour.set("Load Colour", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(chargeColour.set("Charge Colour", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(batteryColour.set("Battery Colour", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(vSync.set("vSync",true));
    parameters.add(fps.set("FPS",ofToString(ofGetFrameRate())));

    vSync.addListener(this, &Gui::vSyncChanged);

    gui.setup(parameters);

    parameters.add(xpos.set("xpos",10));
    parameters.add(ypos.set("ypos",10));

    gui.loadFromFile("settings.xml");

  //  gui.add(fps.setup("FPS",ofGetFrameRate()));
    gui.setPosition(xpos.get(),ypos.get());

}

void Gui::draw()
{
    fps.set(ofToString(ofGetFrameRate()));

    ofSetColor(255);
    gui.draw();
}

void Gui::vSyncChanged(bool & vSync){
    //ofLogNotice() << "Vertical Sync changed";
    ofSetVerticalSync(vSync);
}

