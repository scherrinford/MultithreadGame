#include "Server.h"
#include "Player.h"
#include <ncurses.h>
#include <ctime>
#include <cstdlib>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>

void Server::setMazeOnServer(Maze maze) {
    this->maze = maze;
    maze.mazeGenerator();
}

void Server::setClient(Player player) {

    if(slot1Status == FREE_SLOT){
        player1 = player;
        slot1Status = BUSY_SLOT;
        return;
    }
    else if(slot2Status == FREE_SLOT){
        player2 = player;
        slot2Status = BUSY_SLOT;
        return;
    }
    else{
        //send denied to client
        return;
    }

}

void Server::setColors() {
    init_pair(WALL_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(PLAYER_PAIR, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(BEAST_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(COIN_PAIR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(BASE_PAIR, COLOR_YELLOW, COLOR_GREEN);
}

int Server::serverInit()
{
    srand(time(0));
    ncursesInit();
    maze.mazeGenerator();
    legend.setPosition(width * 2/3, height);
    legend.printLegend();
    legend.printGameStatistic(player1);

    memInit();

    return 0;
}

void Server::memInit() {

    fd = shm_open("shared_mem", O_CREAT | O_RDWR, 0600);
    if(fd== -1) {perror("shm_open"); return;}

    int res = ftruncate(fd, sizeof(struct shared_data_t));
    if(res == -1) {perror("ftruncate"); return;}

    sharedData = (struct shared_data_t *)mmap(NULL, sizeof(struct shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    //sharedData->validateMoveSem = sem_open("validateMoveSem",O_CREAT| O_EXCL, 0600, 0);
    //sharedData->roundTime = sem_open("roundTime",O_CREAT| O_EXCL, 0600, 0);
    //sharedData->saveTreasure = sem_open("saveTreasure",O_CREAT| O_EXCL, 0600, 0);
    //sharedData->waitForClient = sem_open("waitForClient",O_CREAT| O_EXCL, 0600, 0);
    //sharedData->waitForClient = sem_open("roundTime",O_CREAT| O_EXCL, 0600, 0);
    //sharedData->saveTreasure = sem_open("saveTreasure",O_CREAT| O_EXCL, 0600, 0);

    //sem_init(&sharedData->validateMoveSem, 1, 0);
    //sem_init(&sharedData->roundTime, 1, 0);
    //sem_init(&sharedData->criticalSection, 1, 1);
    //sem_init(&sharedData->validateMoveSem, 1, 1);
    //sem_init(&sharedData->waitForClient, 1, 0);
    //sem_init(&sharedData->newClientAsk, 1, 0);

}

void Server::closeConnection() {
    //sem_close(&sharedData->validateMoveSem);
    //sem_close(sharedData->roundTime);
    //sem_close(sharedData->saveTreasure);
    //sem_close(&sharedData->validateMoveSem);
    //sem_close(&sharedData->waitForClient);
    //sem_close(&sharedData->newClientAsk);
    //sem_unlink("validateMoveSem");
    //sem_unlink("roundTime");
    //sem_unlink("saveTreasure");
    //sem_unlink("validateMoveSem");
    //sem_unlink("roundTime");
    //sem_unlink("saveTreasure");
    close(fd);
    shm_unlink("shared_mem");
}

void Server::ncursesInit() {
    initscr();
    start_color();
    setColors();

    win = newwin(128, 128, 0, 0);

    keypad(stdscr, true);
    wrefresh(win);
    delwin(win);

    curs_set(0);
    getmaxyx(stdscr, height, width);

    timeout(1000);
    clear();
    noecho();
    raw();
}
