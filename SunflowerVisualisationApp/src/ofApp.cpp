#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_NOTICE);

    ofBackground(10);
    ofSetFrameRate(60);
    appWidth = ofGetWidth();
    appHeight = ofGetHeight();
    gridDivx = 20;
    gridDivy = 20;
    gridWidth = appWidth/gridDivx;
    gridHeight = appHeight/gridDivy;

    input.resize(gridDivx);
    output.resize(gridDivx);

    data.setup();
    gui.setup();
    cam.setup();

    font.load("fonts/verdana.ttf",10);
}

//--------------------------------------------------------------
void ofApp::update(){


    data.update();
    cam.update();

    input.push_front(data.getNormalisedCharge());
    input.pop_back();
    output.push_front(data.getNormalisedLoad());
    output.pop_back();

}


//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255);

    cam.draw(0,0,appWidth,appHeight);

    for(int i = 0; i < gridDivx; i++) {

        for(int j = 0; j < gridDivy;j++) {

            if(j >= (gridDivy/2)) {
                float val = (1.0f/(gridDivy/2.0f))*(j-(gridDivy/2));

                if(output[i] > val) {
                    ofSetColor(gui.loadColour.get());
                    ofDrawRectangle(i*gridWidth+1,j*gridHeight+1,gridWidth-2, gridHeight-2);
                }

            }
            else {
                float val = (1.0f/(gridDivy/2.0f))*j;

                if(input[i] > val) {
                    ofSetColor(gui.chargeColour.get());
                    ofDrawRectangle(i*gridWidth+1,((gridDivy/2 - 1)-j)*gridHeight+1,gridWidth-2, gridHeight-2);
                }
            }

        }

    }

    ofSetColor(0);
    font.drawString("Instant Load: " + ofToString(data.instantLoad),20,20);
    font.drawString("Max Load: "+ofToString(data.maxLoad) + "  val: "+ofToString(output[0]),20,40);
    font.drawString("Load: " + ofToString(data.load),220,20);
    font.drawString("Instant Charge: " + ofToString(data.instantCharge),420,20);
    font.drawString("Max Charge: "+ofToString(data.maxCharge) + "  val: "+ofToString(input[0]),420,40);
    font.drawString("Charge: " + ofToString(data.charge),620,20);
    font.drawString("Battery Percent: "+ ofToString(data.batteryPercent),20,60);
    font.drawString("Battery Voltage: "+ ofToString(data.batteryVoltage),220,60);

    if(bDrawGui) {
        gui.draw();
    }


}

//--------------------------------------------------------------
void ofApp::updateDMX()
{
#ifdef USE_USB_DMX	
	//asign our colors to the right dmx channels
	dmxData_[1] = 255;
	dmxData_[2] = 0;
	dmxData_[3] = 255;	

	dmxData_[4] = 0;
	dmxData_[5] = 0;
	dmxData_[6] = 255;	
	
	//force first byte to zero (it is not a channel but DMX type info - start code)
	dmxData_[0] = 0;

	if ( ! dmxInterface_ || ! dmxInterface_->isOpen() ) {
		ofLogError() << "Not updating, enttec device is not open.";
	}
	else{
		//send the data to the dmx interface
		dmxInterface_->writeDmx( dmxData_, DMX_DATA_LENGTH );
	}	
#endif
	
}

//--------------------------------------------------------------
void ofApp::setupDMX()
{

#ifdef USE_USB_DMX
       ofLogNotice() << "Setup USB DMX...";
        memset( dmxData_, 0, DMX_DATA_LENGTH );
        dmxInterface_ = ofxGenericDmx::openFirstDevice();
        if ( dmxInterface_ == 0 ) {
            ofLog(OF_LOG_ERROR, "No Enttec Device Found" );
            bDmxUsbActive = false;
        }
        else {
            ofLogNotice("DMX USB opened...");
            bDmxUsbActive = true;
        }
#endif

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

        if(key == ' ') bDrawGui = !bDrawGui;
        if(key == 'f') ofToggleFullscreen();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
