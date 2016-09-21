#pragma once

#include "ofMain.h"
#import "ofxSimplePhysics.h"
#import "ofxSPAutomata.h"
#import "ofxSPWorld.h"
#import "ofxTramontana.h"
#import "ofxDatGui.h"



#define INIT            0
#define IDLE            3
#define RUN             1
#define CALIBRATE       2
#define END             4


class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
 
    ofxSPWorld* world;
    
    ofVec2f orientationsG[7];
    
    ofVec2f gravity;
    ofxTramontanaIOS* controller;
    
    void onConnectionOpen(string &ipAddress);
    void receivedAttitude(vec3d &attitude);
    void receivedOrientation(int &orientation);
    void receivedTouch(vec2d &touch);
    
    //STATE
    int state=INIT;
    void changeState(int newState);
    
    //GAMEELEMENTS
    float startTime;
    float endTime;
    
    ofRectangle target;
    
    
    //GUI
    ofxDatGuiTextInput*     input;
    ofxDatGuiButton*        button;
    ofxDatGui*              endGame;
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void setupGUI();
    
    
    
};
