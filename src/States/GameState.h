#pragma once

#include "State.h"
#include "Snake.h"
#include "ofMain.h"

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
        void drawStartScreen();
        void drawLostScreen();
        void drawBoardGrid();
        void drawScore();
        void changeSong(string filename);

        ofSoundPlayer sound;
        vector<string>songs = {"80s_montage.mp3","cyberpunk_synthwave.mp3","Press_Fuse.mp3","welcome_to_the_grid.mp3"};
        float progress = 0;
        int song_index=0;

        Snake* snake;

        bool foodSpawned = false;
        
        int score = 0;
        int currentFoodX;
        int currentFoodY;

        int boardSizeWidth, boardSizeHeight;
        int cellSize; // Pixels
};