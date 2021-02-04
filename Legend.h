#ifndef UNTITLED3_LEGEND_H
#define UNTITLED3_LEGEND_H

#include "PlayerHandler.h"

#define EMPTY_PAIR     1
#define BEAST_PAIR     2
#define WALL_PAIR      3
#define PLAYER_PAIR    4
#define COIN_PAIR      5
#define BASE_PAIR      6
#define BUSHES_PAIR    7

class Legend{

private:
    int width;
    int height;

public:
    void setPosition(int width, int height);
    void printLegend();
    void printGameStatistic(PlayerHandler player);
};

#endif //UNTITLED3_LEGEND_H
