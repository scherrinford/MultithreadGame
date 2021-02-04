#ifndef UNTITLED3_BEAST_H
#define UNTITLED3_BEAST_H

#include "Maze.h"

#define PLAYER1      '1'
#define PLAYER2      '2'

#define MOVE_UP 0
#define MOVE_DOWN 1
#define MOVE_LEFT 2
#define MOVE_RIGHT 3

class Beast{

    int x;
    int y;
    char tile = EMPTY;

public:
    void moveBeast();
    void setStartPosition();
    int validateMove(int x, int y);
    void displayBeast();
    void clearLastTile(int x, int y);
    int searchPlayer();
    int checkTile(int x1, int y1);
    void followPlayer(int x1, int y1);
    void* beastInitialize(void *args);
    int getPositionX();
    int getPositionY();
};

#endif //UNTITLED3_BEAST_H
