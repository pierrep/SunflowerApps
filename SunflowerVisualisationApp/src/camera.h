#ifndef CAMERA_H
#define CAMERA_H

#include "ofMain.h"


class Camera
{
public:
    Camera();
    void setup();
    void update();
    void draw(float x, float y, float w, float h);

    int camWidth;
    int camHeight;
    ofVideoGrabber vidGrabber;
};

#endif // CAMERA_H
