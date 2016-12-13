#include "camera.h"

Camera::Camera()
{

}

void Camera::update()
{
    vidGrabber.update();
}

void Camera::setup()
{

    camWidth = 1280;  // try to grab at this size.
    camHeight = 720;

    //we can now get back a list of devices.
    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    for(unsigned int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

    vidGrabber.setDeviceID(0);
    //vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(camWidth, camHeight);
}

void Camera::draw(float x, float y, float w, float h)
{
    //vidGrabber.draw(x,y);
    vidGrabber.draw(x,y,w,h);
}
