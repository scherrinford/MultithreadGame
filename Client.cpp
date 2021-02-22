//
// Created by root on 2/20/21.
//
#include <cstdlib>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <signal.h>

#include "Client.h"
#include "Player.h"

int Client::clientJoinToServer(){

    fd = shm_open("shared_mem", O_RDWR, 0600);
    if(fd== -1) return serverDosntExist();

    sharedData = (struct shared_data_t *)mmap(NULL, sizeof(struct shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    roundTime = sem_open("/msg_signal", 0);
    validateMoveSem = sem_open("/validateMoveSem", 0);
    dataSet = sem_open("/data_set", 0);
    waitForClient = sem_open("/waitForClient", 0);
    newClientAsk = sem_open("/newClientAsk", 0);
    synchroSem = sem_open("/synchroSem", 0);

    //semafor na status
    //sharedData->status = LOGIN;

    sharedData->status = CONNECTED;
    return 0;
}

int Client::serverDosntExist() {
    mvprintw(height/2, width/3, "Server doesn't exist\n");
    return 1;
}

void Client::closeConnection() {
    //sem_wait(&sharedData->criticalSection);
    sharedData->status = DISCONNECTED;
    munmap(sharedData, sizeof(shared_data_t));
    sem_close(roundTime);
    sem_close(validateMoveSem);
    sem_close(dataSet);
    sem_close(waitForClient);
    sem_close(newClientAsk);
    sem_close(synchroSem);
    close(fd);
}

void Client::setColors() {
    init_pair(WALL_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(PLAYER_PAIR, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(BEAST_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(COIN_PAIR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(BASE_PAIR, COLOR_YELLOW, COLOR_GREEN);
}

int Client::valicateMove(int y, int x)
{
    int testch = mvinch(y, x);
    return !((testch & A_CHARTEXT) == WALL);
}

void* Client::displayMap(void *object) {
    Client *client = (Client*)object;
    while(true){
        client->sharedData->player.printPlayerView();
        sleep(1);
    }
}

//inicjalizacja ncurses
void Client::ncursesInit() {
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

int Client::runClient(void) {
    srand(time(0));

    ncursesInit();
    int c;

    if(clientJoinToServer()) {
        getch();
        endwin();
        return 1;
    }
    int result= kill(sharedData->server_pid,0);
    if(result==-1 && errno == ESRCH){
        closeConnection();
        serverDosntExist();
        getch();
        endwin();
        return 1;
    }

    legend.setPosition(width * 2/3, height);
    legend.printLegend();
    int tileType=0;
    legend.printGameStatistic(sharedData->player);

    sharedData->player.printPlayerView();

    pthread_t displayMapThread;
    //pthread_create(&displayMapThread, NULL, &Client::displayMap, this);

    sem_wait(dataSet);
    sem_wait(&sharedData->criticalSection); //blokuje przed jednoczesnym zapisaniem do pamieci wsp
    while((c = getch())){

        sem_wait(newClientAsk);
        sem_wait(&sharedData->criticalSection);
        if(c == KEY_UP) {
            sharedData->tmpX = sharedData->player.x;
            sharedData->tmpY = sharedData->player.y - 1;
        }
        else if(c == KEY_DOWN) {
            sharedData->tmpX = sharedData->player.x;
            sharedData->tmpY = sharedData->player.y + 1;
        }
        else if(c == KEY_RIGHT) {
            sharedData->tmpY = sharedData->player.y;
            sharedData->tmpX = sharedData->player.x + 1;
        }
        else if(c == KEY_LEFT) {
            sharedData->tmpX = sharedData->player.x - 1;
            sharedData->tmpY = sharedData->player.y;
        }
        else if(c=='q') {
            sem_post(waitForClient); //klient wyslal odpowiedz status DISCONECTED
            break;
        }
        else{
            sharedData->tmpX = sharedData->player.x;
            sharedData->tmpY = sharedData->player.y;
        }
        sem_post(waitForClient); //klient wyslal odpowiedz

        clock_gettime(CLOCK_REALTIME, &tm);
        tm.tv_sec += 120;
        if(sem_timedwait(validateMoveSem, &tm) != 0) break;

        //sem_wait(validateMoveSem); //klient czeka na potwierdzenie ruchu
        sem_wait(roundTime); //klient czeka na koniec rundy

        sem_wait(synchroSem); //czeka na aktualiczacje ruchu wszystkich obiektow
        legend.printGameStatistic(sharedData->player);
        sharedData->player.printPlayerView();
        refresh();
    }

    closeConnection();
    endwin();

    return 0;
}


int main(void)
{
    Client client;
    client.runClient();
    return 0;
}