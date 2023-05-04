#pragma once
#include "ofMain.h"

class Entity {
    protected:
        int x, y, w, h;
        bool remove = false;
        int ticks = 0;
        ofColor color;
    
    public:
        Entity(int x, int y, int w, int h, ofColor color) {
            this->x = x; 
            this->y = y;
            this->w = w;
            this->h = h;
            this->color = color;
        };

        virtual void update() = 0;
        virtual void draw() = 0;

        int getX() { return x; }
        int getY() { return y; }
        int getWidth() { return w; }
        int getHeight() { return h; }
};


class StaticEntity : public Entity {
    public:
        StaticEntity(int x, int y, int w, int h, ofColor color) : Entity(x, y, w, h, color) {};
        void update() override {};
        void draw() override;
};

class DynamicEntity : public Entity {
    public:
        void update() override;
        void draw() override;
};
