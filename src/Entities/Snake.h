#pragma once
#include <vector>

enum Direction {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        NONE
    };

class Snake {

private:
    std::vector<std::vector<int>> body;
    Direction direction;
    int boardSizeWidth, boardSizeHeight;
    int segmentSize;
    bool crashed;
    bool activeBA = false; // active BETTERAPPLE
    bool activeGM = false; // active GODMODE

public:
    Snake(int segmentSize, int boardSizeW, int boardSizeH);
    ~Snake();

    void update();
    void draw();
    void changeDirection(Direction d);
    void checkSelfCrash();
    void grow();
    void removeTail(); 
    bool isCrashed() {
        return this->crashed;
    }
    std::vector<int> getHead() {
        return this->body[0];
    }
    std::vector<int> getTail() {
        return this->body[this->body.size() - 1];
    }
    std::vector<std::vector<int>> getBody() {
        return this->body;
    }

    void SetActiveBA(bool a) { this->activeBA = a; }
    void SetActiveGM(bool g) { this->activeGM = g; }

    int getActiveGM() { return activeGM; }
};
