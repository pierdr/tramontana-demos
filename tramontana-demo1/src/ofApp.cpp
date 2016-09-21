#include "ofApp.h"
#define TARGET_SIZE 80

//--------------------------------------------------------------
void ofApp::setup(){
    
    world=new ofxSPWorld(ofVec2f(0,0.1));

    
   
    orientationsG[0]=ofVec2f(0,0);
    orientationsG[1]=ofVec2f(0,-0.03);
    orientationsG[2]=ofVec2f(0,0.03);
    orientationsG[3]=ofVec2f(-0.03,0);
    orientationsG[4]=ofVec2f(0.03,0);
    orientationsG[5]=ofVec2f(0,0);
    orientationsG[6]=ofVec2f(0,0);
    
    setupGUI();
    
    target=ofRectangle(ofRandom(TARGET_SIZE,ofGetWidth()-TARGET_SIZE),ofRandom(TARGET_SIZE,ofGetHeight()-TARGET_SIZE),TARGET_SIZE/2,TARGET_SIZE/2);
    
}
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if (e.target == button){
        //printf("%s\n",input->getText().c_str());
        controller=new ofxTramontanaIOS();
        controller->start(input->getText());
        controller->onConnectionOpen(this, &ofApp::onConnectionOpen);
        ofBackground(0);
        changeState(IDLE);
        
    }
}
//--------------------------------------------------------------
void ofApp::onConnectionOpen(string &ipAddress){
    controller->subscribeOrientation(this,&ofApp::receivedOrientation);
    controller->subscribeTouch(this, &ofApp::receivedTouch);
    controller->setColor(ofColor(189,19,8));
}
//--------------------------------------------------------------
void ofApp::receivedOrientation(int &orientation){
   
        world->changeGravity(orientationsG[orientation]);
}

//--------------------------------------------------------------
void ofApp::receivedTouch(vec2d &touch){
    
    changeState(RUN);
    controller->setColor(ofColor(30,190,8));
}
//--------------------------------------------------------------
void ofApp::update(){
    if(state==INIT)
    {
        button->update();
        input->update();
    }
    else if(state==IDLE)
    {
        
    }
    else if(state==RUN)
    {
        world->update();
        
        for(int i=0;i<world->automatas.size();i++)
        {
            if(world->automatas.at(i).isInside(target))
            {
                world->removeAutomata(&(world->automatas.at(i)));
                break;
            }
        }
        if(world->automatas.size()==0)
        {
            changeState(END);
            
            
            
             controller->setColor(ofColor(0,0,0));
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    ofDrawBitmapString(ofToString("tramontana - demo1"), 10, 30);
    if(state==INIT)
    {
        button->draw();
        input->draw();
    }
    else if(state==END)
    {
        ofSetColor(255,255,255);
        ofDrawBitmapString(ofToString(endTime-startTime), 10, 30);
        ofDrawBitmapString("Tap on your controller to start over", 10, 60);
    }
    else
    {
        
        world->display();
        ofSetColor(90, 90, 90,180);
        ofSetLineWidth(3);
        
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofRotate(180-(world->g).angle(ofVec2f(0,1)));
        ofDrawTriangle(-10, 10, 10, 10, 0, -15);
        ofPopMatrix();
        
        //TARGET
        ofDrawRectangle(target);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key=='s')
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
            endGame->setEnabled(true);
            endTime=ofGetElapsedTimef();
            break;
        case RUN:
            startTime=ofGetElapsedTimef();
            world->addAutomata(*new ofxSPAutomata(1));
            world->addAutomata(*new ofxSPAutomata(3));
            world->addAutomata(*new ofxSPAutomata(2));
            world->addAutomata(*new ofxSPAutomata(4));
            break;
        default:
            endGame->setEnabled(false);
            break;
    }
    
    state=newState;

}
//--------------------------------------------------------------
void ofApp::setupGUI(){
    button = new ofxDatGuiButton("Start");
    button->onButtonEvent(this, &ofApp::onButtonEvent);
    
    button->setOrigin(ofGetWidth()/2 - button->getWidth()/2, ofGetHeight()/2 - button->getHeight());
    
    input = new ofxDatGuiTextInput("IPAddress", "192.168.1.2");
    input->setWidth(800);
    
    input->setOrigin(ofGetWidth()/2 - input->getWidth()/2, 240);
    
    
    endGame=new ofxDatGui();
    endGame->setOrigin(ofGetWidth()/2 - input->getWidth()/2, 240);
    endGame->addLabel("Tap on your phone to srart a new game");
    
    endGame->setAutoDraw(false);
    
    // ...
    ofBackground(255);
}
