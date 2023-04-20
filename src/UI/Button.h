#include "ofMain.h"

class Button {
    private:
        int xpos, ypos, width, height;
        bool pressed = false; 
        int red, green, blue; 
        string text; 

    public:
        Button(int xpos, int ypos, int width, int height, string text); 
        void mousePressed(int x, int y);
        void update();
        void draw();
        void reset();
        void SetColor(int red, int green, int blue){
            this->red = red;
            this->green = green;
            this->blue = blue;
        }
        bool wasPressed() {return pressed;}

};