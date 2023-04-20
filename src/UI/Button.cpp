#include "Button.h"

Button::Button(int xpos, int ypos, int width, int height, string text){
    this->xpos = xpos;
    this->ypos = ypos;
    this->width = width;
    this->height = height;
    this->text = text;
}

void Button::update(){
    
}

void Button::reset(){
    pressed = false;
}

void Button::draw(){
    ofSetColor(red,green,blue);
    ofDrawBitmapString(text, xpos, ypos + height / 2);
}

void Button::mousePressed(int x, int y){
    if(xpos + width >= x && x >= xpos &&  ypos + height >= y && y >= ypos){
        pressed = true;
    }
}