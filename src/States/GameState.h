#pragma once

#include "State.h"
#include "Snake.h"
#include "ofMain.h"
#include "entities.h"

class GameState : public State {
    public:
        GameState();
        ~GameState();
        void reset();
        void update();
        void draw();
        void keyPressed(int key);
        void mousePressed(int x, int y, int button);
        void foodSpawner();
        void drawFood();
        void obstacleSpawner();
        void drawObstacles();
        void drawStartScreen();
        void drawLostScreen();
        void drawBoardGrid();
        void drawScore();
        void changeSong(string filename);

        ofSoundPlayer sound;
        vector<string>songs = {"80s_montage.mp3","cyberpunk_synthwave.mp3","Press_Fuse.mp3","welcome_to_the_grid.mp3"};
        float progress = 0;
        int song_index=0;

        vector<ofColor>colors = {
            ofColor::red, ofColor::green, ofColor::blue, ofColor::cyan, ofColor::magenta, ofColor::beige, ofColor::brown, 
            ofColor::coral, ofColor::crimson, ofColor::navy, ofColor::darkGreen, ofColor::hotPink, ofColor::gold, 
            ofColor::orange, ofColor::royalBlue, ofColor::seaGreen, ofColor::turquoise, ofColor::violet, ofColor::yellowGreen
        };
        StaticEntity* wall;

        Snake* snake;

        bool foodSpawned = false;
        bool wallSpawned = false;
        
        int score = 0;
        int currentFoodX;
        int currentFoodY;
        int currentObstacleX;
        int currentObstacleY;

        int boardSizeWidth, boardSizeHeight;
        int cellSize; // Pixels
};