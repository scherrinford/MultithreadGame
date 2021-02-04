#ifndef UNTITLED3_PLAYERHANDLER_H
#define UNTITLED3_PLAYERHANDLER_H

#include "Beast.h"

#define CPU     0
#define HUMAN   1

#define RAND_MOVE_UP 0
#define RAND_MOVE_DOWN 1
#define RAND_MOVE_LEFT 2
#define RAND_MOVE_RIGHT 3

class PlayerHandler{

private:

    int type;
    int playerNumber;
    int spawnX;
    int spawnY;

public:
    int droppedTreasures;
    int carriedCoins = 0;   //niesione monety
    int broughtCoins = 0;   //monety zaniesione do obozowiska
    int deaths = 0;
    int x;
    int y;

    void setType(int type);
    void setPlayerPosition(int x, int y);
    void changePlayerPosition(int c);
    static int validateMove(int x, int y);
    void setStartPosition();
    void displayPlayer();
    void clearLastPosition(int x, int y, int tileType);
    void collectTreasure();
    int isBase(int x, int y);
    int isBushes(int x, int y);
    int isBeast(Beast beas);
    void saveToBudget();
    int getTileType();
    int getPositionX();
    int getPositionY();
    int isCollision(Beast beast);
    void dropTreasure();
    void resetPlayerPosition();

};

#endif //UNTITLED3_PLAYERHANDLER_H
