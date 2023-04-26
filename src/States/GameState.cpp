#include "GameState.h"

//--------------------------------------------------------------
GameState::GameState() {
    sound.load("80s_montage.mp3");    // Loads a sound file (in bin/data/)
    sound.setVolume(1);               // Sets the song volume 
    sound.play();
    foodSpawned = false;
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
        score = 0; 
        setRestart(false);
        resetPower(); 
        isPower = false;
        currentPower = NA; 
    }
    setFinished(false);
    setNextState("");
}
//--------------------------------------------------------------
void GameState::update() {
    ofSoundUpdate();
    if (sound.getPosition() >= .99) {
        if (song_index < (songs.size() - 1)) {
            changeSong(songs[song_index + 1]);
            song_index++;
        } else {
            song_index = 0;
            changeSong(songs[song_index]);
        }
    }
    
    if(startTimer){
        currentPower = NA;
        ticks++;
    }
    if(ticks == 900){
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
    if(ofGetFrameNum() % framenumdivisor == 0) {
        snake->update();
    }

}
//--------------------------------------------------------------
void GameState::draw() {
    drawBoardGrid();
    snake->draw();
    drawFood();
    drawScore();
    drawPower();
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
        case 's':
            sound.setPosition(.99);
            break;
        case 'b':
            usePower();
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
    ticks = 1;
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
    }
}
//--------------------------------------------------------------
void GameState::drawPower(){
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
        if((score - 10) % 300 == 0){
            ticks = 300;
            currentPower = GODMODE;
        } else if((score - 10) % 150 == 0){
            currentPower = BETTERAPPLE;
        } else if((score - 10) % 50 == 0){
            currentPower = SPEEDUP;
        }
    }
    isPower = false;
}
//--------------------------------------------------------------
void GameState::drawFood() {
    ofSetColor(ofColor::red);
    if(foodSpawned) {
        if(score > 0){
            if(score % 300 == 0){
                ofSetColor(ofColor::darkViolet);
                isPower = true;
            } else if (score % 150 == 0){
                ofSetColor(ofColor::yellow);
                isPower = true;
            } else if(score % 50 == 0){
                ofSetColor(ofColor::blue);
                isPower = true;
            }
        }
        ofDrawRectangle(currentFoodX*cellSize, currentFoodY*cellSize, cellSize, cellSize);
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