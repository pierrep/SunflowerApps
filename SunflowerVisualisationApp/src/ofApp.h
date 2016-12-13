#pragma once

#include "ofMain.h"
#include "SolarData.h"
#include "gui.h"
#include "camera.h"

#define USE_USB_DMX 1

#ifdef USE_USB_DMX
#include "ofxGenericDmx.h"
#define DMX_DATA_LENGTH 513
#endif



class ofApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void setupDMX();
        void updateDMX();
        
        /* Enntec DMX USB Pro object */
#ifdef USE_USB_DMX
        DmxDevice* dmxInterface_;
        //our DMX packet (which holds the channel values + 1st byte for the start code)
        unsigned char dmxData_[DMX_DATA_LENGTH];
	bool bDmxUsbActive;
#endif        
        
		ofTrueTypeFont font;

        bool bDrawGui;
        float gridWidth, gridHeight;
        float gridDivx, gridDivy;

        deque<float>   input;
        deque<float>   output;

        Gui gui;
        SolarData data;

        Camera cam;
        float appWidth;
        float appHeight;

};
