#ifndef UNTITLED3_MAZE_H
#define UNTITLED3_MAZE_H
#include <ncurses.h>
#include "Player.h"

#define WALL              '^'
#define EMPTY             ' '
#define BEAST             '*'
#define SMALL_TREASURE    'C'
#define ONE_COIN          'c'
#define BIG_TREASURE      'T'
#define DROPPED_TREASURE  'D'
#define BASE              'A'
#define BUSHES            '#'

#define EMPTY_PAIR     1
#define BEAST_PAIR     2
#define WALL_PAIR      3
#define PLAYER_PAIR    4
#define COIN_PAIR      5
#define BASE_PAIR      6
#define BUSHES_PAIR    7

#define RAND_MOVE_UP 0
#define RAND_MOVE_DOWN 1
#define RAND_MOVE_LEFT 2
#define RAND_MOVE_RIGHT 3

#define WIDTH 40
#define HEIGHT 20

class Maze{

    int width = WIDTH;
    int height = HEIGHT;
    int droppedTreasures = 0;

public:

    void mazeGenerator();
    int getSingleTile(int x, int y);
    void setSingleTile(int x, int y, char tile);
    void mazeRecGenerator(int x, int y);
    void drawBackground();
    void dropTreasure(char tile);
    void setTreasures();
    static int validatePosition(int y, int x);
    void setBushes();
    void setBase();
    void displayPlayer(int y, int x, int playerNumber);
    void dropCarriedCoins(int coins, int x, int y);
};

#endif //UNTITLED3_MAZE_H
