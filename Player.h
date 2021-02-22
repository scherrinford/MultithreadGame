#ifndef UNTITLED3_PLAYERHANDLER_H
#define UNTITLED3_PLAYERHANDLER_H

#include <semaphore.h>
#include "Beast.h"

#define CPU     0
#define HUMAN   1

#define RAND_MOVE_UP 0
#define RAND_MOVE_DOWN 1
#define RAND_MOVE_LEFT 2
#define RAND_MOVE_RIGHT 3

class Player{

private:

    int type;
    int spawnX;
    int spawnY;

public:

    int playerViewTile [5][5];
    int droppedTreasures;
    int carriedCoins = 0;   //niesione monety
    int broughtCoins = 0;   //monety zaniesione do obozowiska
    int deaths = 0;
    int x = 0;
    int y = 0;

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
    int isBeast();
    void saveToBudget();
    int getTileType();
    int getPositionX();
    int getPositionY();
    int isCollision(Beast beast);
    void dropTreasure();
    void resetPlayerPosition();
    void setTileToViewTab();
    int getTile(int y, int x);
    void printPlayerView();

    int playerNumber;

    void clearPlayerStats();
};

#define DISCONNECTED     0
#define CONNECTED        1
#define LOGIN            2

#define UPDATED          0
#define NOT_UPDATED      1

struct shared_data_t{
    Player player;
    int tmpX;
    int tmpY;
    int status = DISCONNECTED;
    pid_t server_pid;

    sem_t criticalSection; //chroni dane przed rownoległa modyfikacha

};

#endif //UNTITLED3_PLAYERHANDLER_H
