#include "Entities.h"

void StaticEntity::draw() {
        ofSetColor(this->color);
        ofDrawRectangle(this->x, this->y, this->w, this->h);
}
//--------------------------------------------------------------
void DynamicEntity::update()  {
        // Update the dynamic entity's position and other properties based on the game logic
        // The implementation for this method will depend on the specific requirements of the game
    }
    
void DynamicEntity::draw()  {
        // Draw the dynamic entity on the screen
        // The implementation for this method will depend on the specific requirements of the game
    }