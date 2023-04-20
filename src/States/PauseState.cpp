#include "PauseState.h"

//--------------------------------------------------------------
PauseState::PauseState() {
    string text = "Continue?";
    unpause = new Button(ofGetWidth() / 2 - 8*text.length()/2, ofGetHeight() / 2, 64, 15, "Continue?");
}   
//--------------------------------------------------------------
PauseState::~PauseState() {
    delete unpause; 
}
//--------------------------------------------------------------
void PauseState::reset() {
    setFinished(false);
    setNextState("");
    unpause->reset();
    return;
}
//--------------------------------------------------------------
void PauseState::update() {
    if(unpause->wasPressed()){
        setNextState("GameState");
        setFinished(true);
        return; 
    }
}
//--------------------------------------------------------------
void PauseState::draw() {
    ofSetColor(ofColor::black);
    ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
    ofSetColor(ofColor::white);
    string text = "The game is currently paused.";
    ofDrawBitmapString(text, ofGetWidth()/2-8*text.length()/2, ofGetHeight()/2-11);
    unpause->SetColor(80,255,64);
    unpause->draw(); 
    return;
}
//--------------------------------------------------------------
void PauseState::keyPressed(int key) {
    if(key == OF_KEY_LEFT || key == OF_KEY_RIGHT || key == OF_KEY_UP || key == OF_KEY_DOWN || key == 'p') {
        setFinished(true);
        setNextState("GameState");
        return;
    }
}
//--------------------------------------------------------------
void PauseState::mousePressed(int x, int y, int button){
    unpause->mousePressed(x, y);
}
//--------------------------------------------------------------