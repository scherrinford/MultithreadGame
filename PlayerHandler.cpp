#include "PlayerHandler.h"
#include <ncurses.h>
#include "Maze.h"
#include "Beast.h"
#include <ctime>
#include <iostream>

void PlayerHandler::setType(int type) {
    PlayerHandler::type = type;
}

void PlayerHandler::setPlayerPosition(int x, int y) {
    PlayerHandler::x = x;
    PlayerHandler::y = y;
}

void PlayerHandler::changePlayerPosition(int c) {
    int x0 = x, y0 = y;
    if(c == KEY_UP && validateMove(y-1,x)) y--;
    if(c == KEY_DOWN && validateMove(y+1,x)) y++;
    if(c == KEY_RIGHT && validateMove(y,x+1)) x++;
    if(c == KEY_LEFT && validateMove(y,x-1)) x--;
    clearLastPosition(x0,y0, getTileType());
}

int PlayerHandler::validateMove(int x, int y) {
    return (mvinch(y, x) & A_CHARTEXT) != WALL;
}

void PlayerHandler::setStartPosition() {
    //srand(time(0));
    while(true){
        spawnX = rand() % WIDTH;
        spawnY = rand() % HEIGHT;
        if (validateMove(spawnX, spawnY)) {
            x = spawnX;
            y = spawnY;
            displayPlayer();
            break;
        }
    }

}

void PlayerHandler::displayPlayer() {
    attron(COLOR_PAIR(PLAYER_PAIR));
    mvaddch(y, x, '1' /* "%d", playerNumber*/);
    attroff(COLOR_PAIR(PLAYER_PAIR));
    refresh();
}

void PlayerHandler::clearLastPosition(int x, int y, int tileType) {
    if(tileType==BASE_PAIR) {
        attron(COLOR_PAIR(BASE_PAIR));
        mvaddch(y, x, 'A');
        attroff(COLOR_PAIR(BASE_PAIR));
    }
    else if(tileType==BUSHES_PAIR)  mvaddch(y, x, '#');
    else mvaddch(y, x, ' ');
    refresh();
}

void PlayerHandler::collectTreasure() {
    if((mvinch(y, x) & A_CHARTEXT) == ONE_COIN) carriedCoins++;
    if((mvinch(y, x) & A_CHARTEXT) == SMALL_TREASURE) carriedCoins += 10;
    if((mvinch(y, x) & A_CHARTEXT) == BIG_TREASURE) carriedCoins += 50;
}

void PlayerHandler::saveToBudget() {
    broughtCoins += carriedCoins;
    carriedCoins = 0;
}

int PlayerHandler::isBase(int x, int y) {
    if((mvinch(y, x) & A_CHARTEXT) == BASE) {
        saveToBudget();
        return 1;
    }
    return 0;
}

int PlayerHandler::isBushes(int x, int y) {
    if((mvinch(y, x) & A_CHARTEXT) == BUSHES) {
        return 1;
    }
    return 0;
}

//sprawdza czy nastąpiła kolizja z bestia
int PlayerHandler::isBeast(Beast beast) {
    if(x == beast.getPositionX() && y == beast.getPositionY()) {
        deaths++;
        dropTreasure();
        resetPlayerPosition();
        return 1;
    }
    return 0;
}

int PlayerHandler::getTileType() {
    if(isBase(x,y)) return BASE_PAIR;
    else if(isBushes(x,y)) return BUSHES_PAIR;
    else return 0;
}

int PlayerHandler::getPositionX() {
    return x;
}

int PlayerHandler::getPositionY() {
    return y;
}

int PlayerHandler::isCollision(Beast beast) {

    return 0;
}

void PlayerHandler::dropTreasure() {
    droppedTreasures = carriedCoins;
    carriedCoins=0;
    attron(COLOR_PAIR(COIN_PAIR));
    mvaddch(y, x, DROPPED_TREASURE);
    attroff(COLOR_PAIR(COIN_PAIR));
}

//respawn gracza do pozycji startowej
void PlayerHandler::resetPlayerPosition() {
    x = spawnX;
    y = spawnY;
}


