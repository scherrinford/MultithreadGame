#include "Maze.h"
#include <ncurses.h>
#include "Legend.h"
#include <ctime>
#include <iostream>
#include "Player.h"

//generowanie planszy
void Maze::mazeGenerator() {
    //srand(time(0));
    attron(COLOR_PAIR(WALL_PAIR));
    drawBackground();
    attroff(COLOR_PAIR(WALL_PAIR));
    mazeRecGenerator(1, 1);
    setTreasures();
    setBushes();
    setBase();
}

//generowanie labiryntu
void Maze::mazeRecGenerator(int x, int y) {
    int unchecked[4] = {RAND_MOVE_LEFT, RAND_MOVE_RIGHT, RAND_MOVE_UP, RAND_MOVE_DOWN };

    for(int i=4; i>0; i--)
    {
        int r = rand()%i;
        int dir = unchecked[r];
        unchecked[r] = unchecked[i-1];

        int dirx, diry;
        if(dir == RAND_MOVE_UP)         { dirx=0; diry=-1; }
        else if(dir == RAND_MOVE_DOWN)  { dirx=0; diry=1;  }
        else if(dir == RAND_MOVE_LEFT)  { dirx=-1; diry=0;  }
        else if(dir==RAND_MOVE_RIGHT) { dirx=1;   diry=0;  }

        if(getSingleTile(x+dirx*2, y+diry*2)==WALL)
        {
            setSingleTile(x+dirx*1, y+diry*1, EMPTY);
            setSingleTile(x+dirx*2, y+diry*2, EMPTY);
            mazeRecGenerator(x+dirx*2, y+diry*2);
        }
    }
}

void Maze::setSingleTile(int x, int y, char tile) {
    if(x<0 || y<0 || x > width || y > height) return;
    else mvaddch(y, x, tile);
}

int Maze::getSingleTile(int x, int y) {
    if(x<0 || y<0 || x > width || y > height) return 0;
    else return mvinch(y, x) & A_CHARTEXT;
}

//generowanie podkladu pod labirynt
void Maze::drawBackground() {
    for (int y = 0; y < height+1; y++) {
        mvhline(y, 0, WALL, width+1);
    }
}

void Maze::dropTreasure(char tile) {
    while(true) {
        int x = rand() % width;
        int y = rand() % height;
        if (validatePosition(y, x)) {
            mvaddch(y, x, tile);
            break;
        }
    }
}

void Maze::setTreasures() {
    //srand(time(0));
    attron(COLOR_PAIR(COIN_PAIR));
    for(int i=0; i<40;i++) dropTreasure(ONE_COIN);
    for(int i=0; i<10;i++) dropTreasure(SMALL_TREASURE);
    for(int i=0; i<4;i++) dropTreasure(BIG_TREASURE);
    attroff(COLOR_PAIR(COIN_PAIR));
}

int Maze::validatePosition(int y, int x)
{
    return (mvinch(y, x) & A_CHARTEXT) != WALL;
}

void Maze::setBushes() {
    for(int i=0; i<80;i++) dropTreasure(BUSHES);
}

void Maze::setBase() {
    attron(COLOR_PAIR(BASE_PAIR));
    dropTreasure(BASE);
    attroff(COLOR_PAIR(BASE_PAIR));
}

void Maze::dropCarriedCoins(int coins, int x, int y) {
    droppedTreasures = coins;
    attron(COLOR_PAIR(COIN_PAIR));
    mvaddch(y, x, DROPPED_TREASURE);
    attroff(COLOR_PAIR(COIN_PAIR));
}

void Maze::displayPlayer(int y, int x, int playerNumber) {
    attron(COLOR_PAIR(PLAYER_PAIR));
    mvprintw(y, x, "%c", playerNumber);
    attroff(COLOR_PAIR(PLAYER_PAIR));
    refresh();
}