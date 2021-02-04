#include <ncurses.h>
#include "Maze.h"
#include <ctime>
#include <iostream>
#include <unistd.h>
#include "Beast.h"

void Beast::setStartPosition() {
    //srand(time(0));
    while(true){
        x = rand() % WIDTH;
        y = rand() % HEIGHT;
        if (validateMove(x, y)) {
            displayBeast();
            break;
        }
    }

}

int Beast::validateMove(int x, int y) {
    return (mvinch(y, x) & A_CHARTEXT) != WALL;
}

void Beast::moveBeast() {
    int unchecked[4] = { RAND_MOVE_UP, RAND_MOVE_DOWN, RAND_MOVE_LEFT, RAND_MOVE_RIGHT};
    int dir;
    while(true)
    {
        dir = searchPlayer();
        int r = rand()%4;
        if(dir==-1){ dir = unchecked[r]; }

        int dirx=0, diry=0;
        if(dir == RAND_MOVE_UP)         { dirx+=0; diry+=-1; }
        else if(dir == RAND_MOVE_DOWN)  { dirx+=0; diry+=1;  }
        else if(dir == RAND_MOVE_LEFT)  { dirx+=-1; diry+=0;  }
        else if(dir == RAND_MOVE_RIGHT) { dirx+=1;  diry+=0;  }

        if (validateMove(x+dirx, y+diry)){
            x += dirx;
            y += diry;
            clearLastTile(x-dirx,y-diry);
            tile = mvinch(y, x) & A_CHARTEXT;
            displayBeast();
            break;
        }


    }


}

void Beast::displayBeast() {
    attron(COLOR_PAIR(BEAST_PAIR));
    mvaddch(y, x, BEAST);
    attroff(COLOR_PAIR(BEAST_PAIR));
    refresh();
}

void Beast::clearLastTile(int x, int y) {
    if(tile==BASE) {
        attron(COLOR_PAIR(BASE_PAIR));
        mvaddch(y, x, tile);
        attroff(COLOR_PAIR(BASE_PAIR));
    }
    else if(tile==BUSHES)  mvaddch(y, x, tile);
    else if(tile==BIG_TREASURE || tile == SMALL_TREASURE || tile == ONE_COIN){
        attron(COLOR_PAIR(COIN_PAIR));
        mvaddch(y, x, tile);
        attroff(COLOR_PAIR(COIN_PAIR));
    }
    else mvaddch(y, x, ' ');
    refresh();
    //mvaddch(y, x, EMPTY);
}

//szuka czy nie ma gracza w zasiegu
int Beast::searchPlayer() {
    int unchecked[4] = {RAND_MOVE_LEFT, RAND_MOVE_RIGHT, RAND_MOVE_UP, RAND_MOVE_DOWN };
    for (int i=1;;i++){
        if(!validateMove(x+i,y)) break; //jak sciana to przerywamy poszukiwanie
        if(checkTile(x+i,y)) return RAND_MOVE_RIGHT;
    }
    for (int i=1;;i++){
        if(!validateMove(x-i,y)) break; //jak sciana to przerywamy poszukiwanie
        if(checkTile(x-i,y)) return RAND_MOVE_LEFT;
    }
    for (int i=1;;i++){
        if(!validateMove(x,y+i)) break; //jak sciana to przerywamy poszukiwanie
        if(checkTile(x,y+i)) return RAND_MOVE_DOWN;
    }
    for (int i=1;;i++){
        if(!validateMove(x,y-i)) break; //jak sciana to przerywamy poszukiwanie
        if(checkTile(x,y-i)) return RAND_MOVE_UP;
    }
    return -1;
}

//sprawdza czy pole jest graczem
int Beast::checkTile(int x1, int y1) {
    return (mvinch(y, x) & A_CHARTEXT) == PLAYER1 || (mvinch(y, x) & A_CHARTEXT) == PLAYER2;
}

void Beast::followPlayer(int x1, int y1) {

}

void* Beast::beastInitialize(void *args) {
    setStartPosition();
    //moveBeast();
    return NULL;
}

int Beast::getPositionX() {
    return x;
}

int Beast::getPositionY() {
    return y;
}
