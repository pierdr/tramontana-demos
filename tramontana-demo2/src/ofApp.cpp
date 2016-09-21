#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    world=new ofxSPWorld(ofVec2f(0,0.1));
    
    button = new ofxDatGuiButton("Start");
    button->onButtonEvent(this, &ofApp::onButtonEvent);
    
    button->setOrigin(ofGetWidth()/2 - button->getWidth()/2, ofGetHeight()/2 - button->getHeight());
    
    input = new ofxDatGuiTextInput("IPAddress", "192.168.1.2");
    input->setWidth(800);
    
    input->setOrigin(ofGetWidth()/2 - input->getWidth()/2, 240);
    
}
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if (e.target == button){
        controller=new ofxTramontanaIOS();
        controller->start(input->getText());
        controller->onConnectionOpen(this, &ofApp::onConnectionOpen);
        ofBackground(0);
    }
}
//--------------------------------------------------------------
void ofApp::onConnectionOpen(string &ipAddress){
    
    controller->subscribeAttitude(15,this,&ofApp::receivedAttitude);
    controller->subscribeTouch(this, &ofApp::receivedTouch);
    controller->setColor(ofColor(189,19,8));
    
}

//--------------------------------------------------------------
void ofApp::receivedAttitude(vec3d &attitude){
    if(state==INIT)
    {
        zeroAttitude=attitude;
        changeState(CALIBRATE);
        controller->makeVibrate();
        controller->setColor(ofColor(20,190,80));
    }
    else if(state==CALIBRATE)
    {
        zeroAttitude=attitude;
    }
    currentAttitude=attitude;
    
}
//--------------------------------------------------------------
void ofApp::receivedTouch(vec2d &touch){
    
    if(state==RUN)
    {
        ofVec2f v=ofVec2f(ofMap(zeroAttitude.z-currentAttitude.z, -1, 1, 0, ofGetWidth()),ofMap(zeroAttitude.y-currentAttitude.y, -1, 1, 0, ofGetHeight()));
        
        
        for(int i=0;i<world->automatas.size();i++)
        {
            if(v.distance(world->automatas.at(i).location)<10)
            {
                world->removeAutomata(&(world->automatas.at(i)));
                controller->makeVibrate();
                if(world->automatas.size()==0)
                {
                    changeState(END);
                }
            }
        }
    }
    else    if(state==END)
    {
        changeState(RUN);
    }
    else if(state==CALIBRATE)
    {
        
        changeState(RUN);
        controller->makeVibrate();
        controller->setColor(ofColor(20,190,80));
    }
}
//--------------------------------------------------------------
void ofApp::update(){
    
    if(state==INIT)
    {
        button->update();
        input->update();
    }
    else
    {
        world->update();
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    ofDrawBitmapString(ofToString("tramontana - demo2"), 10, 30);
    
    
    if(state==INIT)
    {
        button->draw();
        input->draw();
    }
    else if(state==END)
    {
        
        ofDrawBitmapString(ofToString(endTime-startTime), 10, 60);
        ofDrawBitmapString("Tap on your controller to start over", 10, 90);
    }
    else
    {
        if(state==CALIBRATE)
        {
            ofDrawBitmapString(ofToString("get confortable - tap on your controller to start to start"), 10, 60);
        }
        world->display();
        
        
        
        ///CURSOR
        
        float xTmp=ofMap(zeroAttitude.z-currentAttitude.z, -1, 1, 0, ofGetWidth());
        float yTmp=ofMap(zeroAttitude.y-currentAttitude.y, -1, 1, 0, ofGetHeight());
        
        ofSetColor(255, 0, 0,180);
        ofSetLineWidth(3);
        ofPushMatrix();
        ofTranslate(xTmp, yTmp);
        ofRotate(gravity.angle(ofVec2f(0,1)));
        ofDrawLine(-30, 0, 30,0);
        ofDrawLine(0, -30, 0,30);
        ofNoFill();
        ofDrawRectangle(-10, -10, 20, 20 );
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofRotate(180-(world->g).angle(ofVec2f(0,1)));
        ofDrawTriangle(-10, 10, 10, 10, 0, -15);
        ofPopMatrix();
        
        if(state==RUN)
        {
            if(ofGetElapsedTimef()-changeGravityEvent>1.0)
            {
                changeGravityEvent=ofGetElapsedTimef();
                world->changeGravity(ofVec2f((ofRandom(1.0)>0.5)?-0.01:0.01,(ofRandom(1.0)>0.5)?-0.01:0.01));
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='a')
    {
        gravity=ofVec2f(0,-0.01);
    }
    else if(key=='b')
    {
        changeState(END);
    }
    else if(key=='z')
    {
        state=RUN;
        
    }
    else if(key=='s')
    {
        ofSetFullscreen(true);
    }
}
//--------------------------------------------------------------
void ofApp::changeState(int newState){
    if(state==newState)
    {
        return;
    }
    switch(newState)
    {
        case END:
            
            break;
        case RUN:
            startTime=ofGetElapsedTimef();
            world->addAutomata(*new ofxSPAutomata(1));
            world->addAutomata(*new ofxSPAutomata(3));
            world->addAutomata(*new ofxSPAutomata(2));
            world->addAutomata(*new ofxSPAutomata(4));
            break;
        default:
            
            break;
    }
    
    state=newState;
    
}

