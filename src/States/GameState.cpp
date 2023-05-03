#include "GameState.h"

//--------------------------------------------------------------
GameState::GameState() {
    sound.load("80s_montage.mp3");    // Loads a sound file (in bin/data/)
    sound.setVolume(1);               // Sets the song volume 
    sound.play();
    foodSpawned = false;
    wallSpawned = false;
    cellSize = 25;
    boardSizeWidth = 55;
    boardSizeHeight = 30;
    snake = new Snake(cellSize, boardSizeWidth, boardSizeHeight);
    currentPower = NA; 
}
//--------------------------------------------------------------
GameState::~GameState() {
    delete snake;
}
//--------------------------------------------------------------
void GameState::reset() {
    if(getRestart()){
        delete snake;
        snake = new Snake(cellSize, boardSizeWidth, boardSizeHeight);
        foodSpawned = false;
        wallSpawned = false;
        score = 0; 
        setRestart(false);
        resetPower(); 
        isPower = false;
        currentPower = NA; 
        walls.clear();
    }
    setFinished(false);
    setNextState("");
}
//--------------------------------------------------------------
void GameState::update() {
    decay_ticks++; 

    ofSoundUpdate();
    if (sound.getPosition() >= .99) {
        if (song_index < (int)(songs.size() - 1)) {
            changeSong(songs[song_index + 1]);
            song_index++;
        } else {
            song_index = 0;
            changeSong(songs[song_index]);
        }
    }

    if(decay_ticks % 60 == 0 && red_decay > 150 && green_decay < 75){
        red_decay -= 3.5; // (255 - 150) / 30 = 3.5 * sec ; to update the color accordingly
        green_decay += 2.5; // 75 / 30 = 2.5 * sec 
    } 

    if(decay_ticks % 1800 == 0 && !isPower){  // (60 updates * sec) * (30 sec) = 1800
        decay_ticks = 0;
        foodSpawned = false;
    }

    if(startTimer){
        currentPower = NA;
        power_ticks++;
    }
    if(power_ticks == 900){
        resetPower();
    }

    if(snake->isCrashed()) {
        this->setNextState("LoseState");
        this->setRestart(true);
        this->setFinished(true);
        return;
    }

    if(snake->getHead()[0] == currentFoodX && snake->getHead()[1] == currentFoodY) {
        snake->grow();
        score += 10;
        foodSpawned = false;
        if(isPower){
            setPower();
        }
    }
    
    foodSpawner();
    obstacleSpawner();

    for(StaticEntity* wall : walls) {
        for(int j = 0; j < wall->getWidth() / cellSize; j++){
            if(snake->getHead()[0] == wall->getX() / cellSize + j && snake->getHead()[1] == wall->getY() / cellSize){
                this->setNextState("LoseState");
                this->setFinished(true); 
                this->setRestart(true);
            }
        }

        for(int k = 0; k < wall->getHeight() / cellSize; k++){
            if(snake->getHead()[0] == wall->getX() / cellSize && snake->getHead()[1] == wall->getY() / cellSize+ k){
                this->setNextState("LoseState");
                this->setFinished(true);
                this->setRestart(true);
            }
        }
    }

    if(ofGetFrameNum() % framenumdivisor == 0) {
        snake->update();
    }

}
//--------------------------------------------------------------
void GameState::draw() {
    drawBoardGrid();
    snake->draw();
    if(score % 50 == 0 && score > 0){
        drawPower();
    } else {
        drawFood();
    }
    drawScore();
    drawObstacles();
    drawPowerIndicator();
    if(startTimer){
        ofSetColor(ofColor::white);
        ofDrawBitmapString("Your powerup is active, you cannot pickup any other powerups in the meantime!", 0, 30);
    }
}
//--------------------------------------------------------------
void GameState::keyPressed(int key) {

    //if(key != OF_KEY_LEFT && key != OF_KEY_RIGHT && key != OF_KEY_UP && key != OF_KEY_DOWN) { return; }

    switch(key) {
        case OF_KEY_LEFT:
            snake->changeDirection(LEFT);
            break;
        case OF_KEY_RIGHT:
            snake->changeDirection(RIGHT);
            break;
        case OF_KEY_UP:
            snake->changeDirection(UP);
            break;
        case OF_KEY_DOWN:
            snake->changeDirection(DOWN);
            break;
        case 'u':
            snake->removeTail();
            break; 
        case 'a':
            score += 10;
            break;
        case 'p':
            setNextState("PauseState");
            setFinished(true);
            break;
        case 's':
            sound.setPosition(.99);
            break;
        case 'b':
            usePower();
            break;
        case 'w':
            wallSpawned = false;
            break;
    }
}
//--------------------------------------------------------------
void GameState::usePower(){
    startTimer = true;
    switch(currentPower){
        case SPEEDUP:
            score -= 50; 
            framenumdivisor = 5;
            break;
        case BETTERAPPLE:
            score -= 100;
            snake->SetActiveBA(true);
            break;
        case GODMODE:
            score -= 150;
            snake->SetActiveGM(true);
            break;
        case NA:
            startTimer = false;
            break;
    }
}
//--------------------------------------------------------------
void GameState::resetPower(){
    startTimer = false;
    power_ticks = 1;
    framenumdivisor = 10;
    snake->SetActiveBA(false);
    snake->SetActiveGM(false);
}
//--------------------------------------------------------------
void GameState::foodSpawner() {
    if(!foodSpawned) {
        bool isInSnakeBody;
        do {
            isInSnakeBody = false;
            currentFoodX = ofRandom(1, boardSizeWidth-1);
            currentFoodY = ofRandom(1, boardSizeHeight-1);
            for(unsigned int i = 0; i < snake->getBody().size(); i++) {
                if(currentFoodX == snake->getBody()[i][0] and currentFoodY == snake->getBody()[i][1]) {
                    isInSnakeBody = true;
                }
            }
        } while(isInSnakeBody);
        foodSpawned = true;
        red_decay = 255;
        green_decay = 0;
        decay_ticks = 0;
    }
}
//--------------------------------------------------------------
void GameState::drawPowerIndicator(){
    ofSetColor(ofColor::white);
    if(currentPower == SPEEDUP){
        ofDrawBitmapString("Speed up is available! Press B to use it.", 0, 15);
    } else if(currentPower == BETTERAPPLE){
        ofDrawBitmapString("Better Apple is available! Press B to use it.", 0, 15);
    } else if(currentPower == GODMODE){
        ofDrawBitmapString("GODMODE is available! Press B to use it!", 0, 15);
    }
}
//--------------------------------------------------------------
void GameState::setPower(){
    if(isPower){
        if((score - 10) % 150 == 0){
            power_ticks = 300;
            currentPower = GODMODE;
        } else if((score - 10) % 100 == 0){
            currentPower = BETTERAPPLE;
        } else if((score - 10) % 50 == 0){
            currentPower = SPEEDUP;
        }
    }
    isPower = false;
}
//--------------------------------------------------------------
void GameState::drawFood() {
    ofSetColor(red_decay, green_decay, 0);
    if(foodSpawned) {
        ofDrawRectangle(currentFoodX*cellSize, currentFoodY*cellSize, cellSize, cellSize);
    }
}
//--------------------------------------------------------------
void GameState::drawPower() {
    if(foodSpawned) {
        if(score % 150 == 0){
            ofSetColor(ofColor::darkViolet);
            isPower = true;
        } else if (score % 100 == 0){
            ofSetColor(ofColor::yellow);
        } else if(score % 50 == 0){
            ofSetColor(ofColor::blue);
        }
        isPower = true;
        ofDrawRectangle(currentFoodX*cellSize, currentFoodY*cellSize, cellSize, cellSize);
    }
}
//--------------------------------------------------------------
void GameState::obstacleSpawner() {
    if (!wallSpawned) {
        for(int i = 0; i < 10; i++){
            bool isInSnakeBody;
            do {
                isInSnakeBody = false;
                currentObstacleX = ofRandom(1, boardSizeWidth-1);
                currentObstacleY = ofRandom(1, boardSizeHeight-1);
                for(unsigned int i = 0; i < snake->getBody().size(); i++) {
                    if(currentObstacleX == snake->getBody()[i][0] and currentObstacleY == snake->getBody()[i][1]) {
                        isInSnakeBody = true;
                    }
                }
            } while(isInSnakeBody);
            wallSpawned = true;
            
            int randomWidth = ceil(ofRandom(1, 5))*cellSize;
            int randomHeight = ceil(ofRandom(1, 5))*cellSize;

            if(ofRandom(1) < 0.5) {
                randomWidth = cellSize; // Set width to 1 and height to a random value
            } else {
                randomHeight = cellSize; // Set height to 1 and width to a random value
            }
            walls.push_back(new StaticEntity(currentObstacleX*cellSize, currentObstacleY*cellSize, randomWidth, randomHeight, colors[ofRandom(colors.size()-1)]));
        } 
    }
}
//--------------------------------------------------------------
void GameState::drawObstacles() {
    for (StaticEntity* wall : walls){
        if (wall != nullptr) {  // Draw wall only if it exists
            wall->draw();
        }
    }
}

//--------------------------------------------------------------
void GameState::drawStartScreen() {
    ofSetColor(ofColor::black);
    ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
    ofSetColor(ofColor::white);
    string text = "Press any arrow key to start.";
    ofDrawBitmapString(text, ofGetWidth()/2-8*text.length()/2, ofGetHeight()/2-11);
    return;
}
//--------------------------------------------------------------
void GameState::drawLostScreen() {
    ofSetColor(ofColor::black);
    ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
    ofSetColor(ofColor::white);
    string text = "You lost! Press any arrow key to play again";
    string text2 = "or press ESC to exit.";
    ofDrawBitmapString(text, ofGetWidth()/2-8*text.length()/2, ofGetHeight()/2-11);
    ofDrawBitmapString(text2, ofGetWidth()/2-8*text2.length()/2, ofGetHeight()/2+2);
    return;
}
//--------------------------------------------------------------
void GameState::drawBoardGrid() {
    
    ofDrawGrid(25, 64, false, false, false, true);
    
    // ofSetColor(ofColor::white);
    // for(int i = 0; i <= boardSize; i++) {
    //     ofDrawLine(i*cellSize, 0, i*cellSize, ofGetHeight());
    //     ofDrawLine(0, i*cellSize, ofGetWidth(), i*cellSize);
    // }
}
//--------------------------------------------------------------
void GameState::drawScore() {
    ofSetColor(ofColor::white);
    string scoreStr = "SCORE: " + to_string(score);
    ofDrawBitmapString(scoreStr, ofGetWidth()/2, 10);
}
//--------------------------------------------------------------
void GameState::changeSong(string filename){ 
    sound.load(filename);
    sound.play();
}
//--------------------------------------------------------------
void GameState::mousePressed(int x, int y, int button){
    
}
//--------------------------------------------------------------