#include <iostream>
#include <ncurses.h>
#include <pthread.h>
#include <ctime>
#include <unistd.h>
#include "Maze.h"
#include "Legend.h"
#include "PlayerHandler.h"
#include "Beast.h"


#define RAND_MOVE_UP 0
#define RAND_MOVE_DOWN 1
#define RAND_MOVE_LEFT 2
#define RAND_MOVE_RIGHT 3



int valicateMove(int y, int x)
{
    int testch = mvinch(y, x);
    return !((testch & A_CHARTEXT) == WALL);
}

void* beastInitialize(void *args) {
    Beast beast;
    beast.setStartPosition();
    //for(int i = 0; i<1000; i++)
    while (true){
        beast.moveBeast();
        usleep(100* 1000);
    }
    return NULL;
}


int main()
{
    srand(time(0));
    initscr();
    start_color();
    init_pair(WALL_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(PLAYER_PAIR, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(BEAST_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(COIN_PAIR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(BASE_PAIR, COLOR_YELLOW, COLOR_GREEN);
    WINDOW* win;
    win = newwin(128, 128, 128, 128);
    keypad(stdscr, true);
    wrefresh(win);
    delwin(win);

    curs_set(0);
    //init_pair(1,COLOR_MAGENTA,COLOR_WHITE);
    int c,height, width, x  ,y;
    getmaxyx(stdscr, height, width);
    y = height/2;
    x = width/2;
    //mazeGenerator(128,128);
    clear();
    noecho();
    raw();

    PlayerHandler player;
    Maze maze;
    Legend legend;
    Beast beast;
    maze.mazeGenerator();
    legend.setPosition(width * 2/3, height);
    legend.printLegend();
    int tileType=0;
    //mvprintw(height/2, width *2/3, "Player 1 points: %d", player.carriedCoins);
    legend.printGameStatistic(player);
    //draw_map();
    //move(x,y);
    //printw("X");

    pthread_t thread;
    pthread_create(&thread, NULL, beastInitialize, NULL);


    //player.setPlayerPosition(x,y);
    player.setStartPosition();
    //beast.setStartPosition();
    //x = player.x;
    //y = player.y;
    while((c = getch())){
        //beast.setStartPosition();
        player.clearLastPosition(player.x,player.y, tileType);
        //clear();
        if(c == KEY_UP && valicateMove(player.y-1,player.x)) player.y--;
        if(c == KEY_DOWN && valicateMove(player.y+1,player.x)) player.y++;
        if(c == KEY_RIGHT && valicateMove(player.y,player.x+1)) player.x++;
        if(c == KEY_LEFT && valicateMove(player.y,player.x-1)) player.x--;
        if(c=='x') break;
        tileType = player.getTileType();
        player.collectTreasure();
        player.isBeast(beast);
        legend.printGameStatistic(player);
        player.displayPlayer();

        refresh();
    }

    //getch();
    endwin();
    return 0;
}
