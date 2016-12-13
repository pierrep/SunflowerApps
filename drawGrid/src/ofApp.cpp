#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(100);
    ofSetFrameRate(30);
    gridDivx = 20;
    gridDivy = 20;
    gridWidth = ofGetWidth()/gridDivx;
    gridHeight = ofGetHeight()/gridDivy;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
bool bToggle = false;

    for(int i = 0; i < gridDivx; i++) {
        for(int j = 0; j < gridDivy;j++) {
            if(bToggle)
                ofSetColor(150,150,150);
            else {
                if(j >= (gridDivy/2)) {
                    ofSetColor(0,0,255);
                }
                else
                    ofSetColor(255,0,255);
            }
            //bToggle = !bToggle;
            ofDrawRectangle(i*gridWidth+1,j*gridHeight+1,gridWidth-2, gridHeight-2);
        }
        //bToggle = !bToggle;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
