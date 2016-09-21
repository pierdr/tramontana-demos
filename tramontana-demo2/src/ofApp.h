#pragma once

#include "ofMain.h"
#import "ofxSimplePhysics.h"
#import "ofxSPAutomata.h"
#import "ofxSPWorld.h"
#import "ofxTramontana.h"
#import "ofxDatGui.h"



#define INIT        0
#define RUN         1
#define CALIBRATE   2
#define END         3

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
    ofxSPWorld* world;
    
    vec3d zeroAttitude;
    vec3d currentAttitude;
    
    ofVec2f gravity;
    ofxTramontanaIOS* controller;
    
    void onConnectionOpen(string &ipAddress);
    void receivedAttitude(vec3d &attitude);
    void receivedOrientation(int &orientation);
    void receivedTouch(vec2d &touch);
    
    int state=INIT;
    void changeState(int newState);
    
    ofxDatGuiTextInput* input;
    
    ofxDatGuiButton* button;
    void onButtonEvent(ofxDatGuiButtonEvent e);
    
    
    
    float changeGravityEvent;
    
    //GAMEELEMENTS
    float startTime;
    float endTime;
};
