#include "Player.h"
#include <ncurses.h>
#include "Maze.h"
#include "Beast.h"
#include <ctime>
#include <iostream>

void Player::setType(int type) {
    Player::type = type;
}

void Player::setPlayerPosition(int x, int y) {
    Player::x = x;
    Player::y = y;
}

void Player::changePlayerPosition(int c) {
    int x0 = x, y0 = y;
    if(c == KEY_UP && validateMove(y-1,x)) y--;
    if(c == KEY_DOWN && validateMove(y+1,x)) y++;
    if(c == KEY_RIGHT && validateMove(y,x+1)) x++;
    if(c == KEY_LEFT && validateMove(y,x-1)) x--;
    clearLastPosition(x0,y0, getTileType());
}

int Player::validateMove(int x, int y) {
    return (mvinch(y, x) & A_CHARTEXT) != WALL;
}

void Player::setStartPosition() {
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

void Player::displayPlayer() {
    //int tileType = getTileType();
    attron(COLOR_PAIR(PLAYER_PAIR));
    mvprintw(y, x, "%c", playerNumber);
    attroff(COLOR_PAIR(PLAYER_PAIR));
    refresh();
    //clearLastPosition(tileType);
}

void Player::clearLastPosition(int x, int y, int tileType) {
    if(tileType==BASE_PAIR) {
        attron(COLOR_PAIR(BASE_PAIR));
        mvaddch(y, x, 'A');
        attroff(COLOR_PAIR(BASE_PAIR));
    }
    else if(tileType==BUSHES_PAIR)  mvaddch(y, x, '#');
    else mvaddch(y, x, ' ');
    refresh();
}

void Player::collectTreasure() {
    if((mvinch(y, x) & A_CHARTEXT) == ONE_COIN) carriedCoins++;
    if((mvinch(y, x) & A_CHARTEXT) == SMALL_TREASURE) carriedCoins += 10;
    if((mvinch(y, x) & A_CHARTEXT) == BIG_TREASURE) carriedCoins += 50;
}

void Player::saveToBudget() {
    broughtCoins += carriedCoins;
    carriedCoins = 0;
}

int Player::isBase(int x, int y) {
    if((mvinch(y, x) & A_CHARTEXT) == BASE) {
        saveToBudget();
        return 1;
    }
    return 0;
}

int Player::isBushes(int x, int y) {
    return (mvinch(y, x) & A_CHARTEXT) == BUSHES;
}

//sprawdza czy nastąpiła kolizja z bestia
int Player::isBeast() {
    //if(x == beast.getPositionX() && y == beast.getPositionY()) {
        deaths++;
        dropTreasure();
        resetPlayerPosition();
        //displayPlayer();
        return 1;
    //}
    return 0;
}

int Player::getTileType() {
    if(isBase(x,y)) return BASE_PAIR;
    else if(isBushes(x,y)) return BUSHES_PAIR;
    else return 0;
}

int Player::getPositionX() {
    return x;
}

int Player::getPositionY() {
    return y;
}

void Player::dropTreasure() {
    droppedTreasures = carriedCoins;
    carriedCoins=0;
    attron(COLOR_PAIR(COIN_PAIR));
    mvaddch(y, x, DROPPED_TREASURE);
    attroff(COLOR_PAIR(COIN_PAIR));
}

//respawn gracza do pozycji startowej
void Player::resetPlayerPosition() {
    x = spawnX;
    y = spawnY;
}

void Player::setTileToViewTab(){

    for(int i=y-2, tabY=0; i<=y+2; i++, tabY++){
        for(int j=x-2, tabX=0; j<=x+2; j++, tabX++){
            playerViewTile[tabY][tabX] = getTile(i,j);
        }
    }
    //playerViewTile[2][2] = PLAYER1;
}

int Player::getTile(int y, int x) {
    return mvinch(y, x) ;//& A_CHARTEXT;
}

void Player::printPlayerView() {
    for(int i=0, posY=3; i<5; i++, posY++){
        for(int j=0, posX=50; j<5; j++, posX++){
            mvaddch(posY, posX, playerViewTile[i][j]);
        }
    }
    attron(COLOR_PAIR(PLAYER_PAIR));
    mvprintw(5, 52, "%d", playerNumber);
    attroff(COLOR_PAIR(PLAYER_PAIR));
    //mvaddch(1, 1, playerViewTile[1][1]);
}
