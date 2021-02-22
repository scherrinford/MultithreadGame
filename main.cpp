#include <iostream>
#include <ncurses.h>
#include <pthread.h>
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "Maze.h"
#include "Legend.h"
#include "Player.h"
#include "Beast.h"


#define RAND_MOVE_UP 0
#define RAND_MOVE_DOWN 1
#define RAND_MOVE_LEFT 2
#define RAND_MOVE_RIGHT 3

WINDOW* win;
int height;
int width;

Maze maze;
Legend legend;
Player player1, player2;

//Uchwyt na pamięć współdzielona
int fd;
struct shared_data_t *sharedData;

sem_t *newClientAsk; //informuje ze client wyslal zapytanie o ruch
sem_t *validateMoveSem; //przekazuje info ze ruch jest poprawny
sem_t *waitForClient; //czeka na ruch klienta
sem_t *roundTime; //inicjuje runde
sem_t *dataSet;
sem_t *synchroSem; //czeka az wszyskr obiekty zaktualizuja swój ruch

int valicateMove(int y, int x);
int isCollision(Beast *beast, Player player);
void* beastInitialize(void *args);
void* player2Initialize(void *args);
void* timer(void *args);
void memInit();
void closeConnection();
void setColors();

//sprawdzenie poprawnosci ruchu
int valicateMove(int y, int x)
{
    int testch = mvinch(y, x);
    return !((testch & A_CHARTEXT) == WALL);
}

//kolizja bestii z graczem
int isCollision(Beast beast, Player player)
{
    if(player.getPositionY() == beast.getPositionY() && player.getPositionX() == beast.getPositionX() )
        return true;
    else return false;
}

//watek obslugujacy bestie
void* beastInitialize(void *args) {
    Beast beast;
    beast.setStartPosition();
    while (true){
        beast.moveBeast();
        sem_wait(roundTime); //czeka na koniec rundy
        beast.displayBeast();
        if(isCollision(beast, player1)){
            maze.dropCarriedCoins(player1.carriedCoins, player1.x, player1.y);
            player1.isBeast();
            maze.displayPlayer(player1.y, player1.x, PLAYER1);
        }
        if(isCollision(beast, player2)){
            maze.dropCarriedCoins(player2.carriedCoins, player2.x, player2.y);
            player2.isBeast();
            sem_wait(&sharedData->criticalSection);
            sharedData->player = player2;
            sem_post(&sharedData->criticalSection);
            maze.displayPlayer(player2.y, player2.x, PLAYER1);
        }
        sleep(1);
    }
    return NULL;
}

//TODO jakas refaktoryzacja teges
//watek obslugujacy 2 gracza
void* player2Initialize(void *args) {
    player2.setStartPosition();
    player2.playerNumber = 2;
    player2.setTileToViewTab();

    sem_wait(&sharedData->criticalSection);
    sharedData->player = player2;
    sharedData->tmpX = sharedData->player.x;
    sharedData->tmpY = sharedData->player.y;
    sem_post(&sharedData->criticalSection);
    sem_post(dataSet);
    sem_post(&sharedData->criticalSection);

    int tileType=0;
    while(true){

        sem_post(newClientAsk);
        if(sharedData->status==LOGIN){
            mvprintw(1,70,"player 2 is conected");
            sharedData->status=CONNECTED;
        }
        sem_wait(waitForClient); //czeka na ruch klienta
        if(sharedData->status==CONNECTED){

            player2.clearLastPosition(player2.x, player2.y, tileType);

            if(valicateMove(sharedData->tmpY, sharedData->tmpX)){
                if(player2.isBushes(sharedData->tmpX, sharedData->tmpY))
                    sem_wait(roundTime); //blokuje gracza na runde
                player2.x = sharedData->tmpX;
                player2.y = sharedData->tmpY;
            }
            sem_post(validateMoveSem);

            player2.setTileToViewTab();
            player2.collectTreasure();

            sem_wait(roundTime); //czeka na koniec rundy
            player2.setTileToViewTab();

            sharedData->player = player2;

            tileType = player2.getTileType();
            maze.displayPlayer(player2.y, player2.x, PLAYER2);
            legend.printGameStatistic2(player2);
            sem_post(&sharedData->criticalSection);
        }
        if(sharedData->status==DISCONNECTED){
            mvprintw(1,70,"player 2 is disconected");
            mvaddch(player2.y,player2.x,EMPTY);
            legend.clearGameStatistic(player2);
            player2.clearLastPosition(player2.y, player2.x, EMPTY);
            sem_post(&sharedData->criticalSection);
            sem_post(dataSet);
        }

    }
    return NULL;
}

//odlicza czas rundy
void* timer(void *args) {
    while(true){
        sleep(0.5);
        //sem_post(&sharedData->roundTime);
        sem_post(roundTime);
        sleep(0.5);
        sem_post(synchroSem);
    }
}

void* synchronization(void *args) {
    while(true){
        sleep(1);
        //sem_post(&sharedData->roundTime);
        //sem_post(synchroSem);
    }
}

//inicjalizacja wspoldzielonej pamieci i semaforow
void memInit() {

    validateMoveSem = sem_open("validateMoveSem",O_CREAT, 0600, 0);
    roundTime = sem_open("/msg_signal",O_CREAT, 0600, 0);
    dataSet = sem_open("/data_set",O_CREAT, 0600, 0);
    waitForClient = sem_open("/waitForClient",O_CREAT, 0600, 0);
    newClientAsk = sem_open("/newClientAsk",O_CREAT, 0600, 0);
    synchroSem = sem_open("/synchroSem",O_CREAT, 0600, 0);

    fd = shm_open("shared_mem", O_CREAT | O_RDWR, 0600);
    if(fd== -1) {perror("shm_open"); return;}

    int res = ftruncate(fd, sizeof(struct shared_data_t));
    if(res == -1) {perror("ftruncate"); return;}

    sharedData = (struct shared_data_t *)mmap(NULL, sizeof(struct shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sharedData->server_pid= getpid();

    sem_init(&sharedData->criticalSection, 1, 1);

}

//inicjalizacja ncurses
void ncursesInit() {
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

//ustawienie kolorow
void setColors() {
    init_pair(WALL_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(PLAYER_PAIR, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(BEAST_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(COIN_PAIR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(BASE_PAIR, COLOR_YELLOW, COLOR_GREEN);
}

//zamkniecie wspoldzielenia pamieci i semaforow
void closeConnection() {
    sem_close(&sharedData->criticalSection);
    sem_unlink("validateMoveSem");
    sem_unlink("/msg_signal");
    sem_unlink("/data_set");
    sem_unlink("/newClientAsk");
    sem_unlink("/waitForClient");
    sem_unlink("/synchroSem");
    close(fd);
    shm_unlink("shared_mem");
}

int main()
{
    srand(time(0));

    ncursesInit();

    //Beast beast;
    int c, tileType=0;

    maze.mazeGenerator();
    legend.setPosition(width * 2/3, height);
    legend.printLegend();
    legend.printGameStatistic(player1);

    memInit();

    pthread_t beastThread, player2Thread, timerThread, synchroThread;
    pthread_create(&beastThread, NULL, beastInitialize, NULL);
    pthread_create(&player2Thread, NULL, player2Initialize, NULL);
    pthread_create(&timerThread, NULL, timer, NULL);
    //pthread_create(&synchroThread, NULL, synchronization, NULL);

    player1.setStartPosition();
    maze.displayPlayer(player1.y, player1.x, PLAYER1);
    player1.playerNumber = 1;
    player1.setTileToViewTab();
    player1.printPlayerView();
    while((c = getch())){
        player1.clearLastPosition(player1.x, player1.y, tileType);
        //clear();
        if(c == KEY_UP && valicateMove(player1.y - 1, player1.x)) player1.y--;
        if(c == KEY_DOWN && valicateMove(player1.y + 1, player1.x)) player1.y++;
        if(c == KEY_RIGHT && valicateMove(player1.y, player1.x + 1)) player1.x++;
        if(c == KEY_LEFT && valicateMove(player1.y, player1.x - 1)) player1.x--;
        if(c=='q') break;
        tileType = player1.getTileType();
        player1.collectTreasure();

        if(player1.isBushes(player1.x, player1.y))
            sem_wait(roundTime); //blokuje gracza na runde
        //player1.isBeast(beast);
        //sem_wait(&sharedData->roundTime); //czeka na koniec rundy

        sem_wait(roundTime); //czeka na koniec rundy
        player1.setTileToViewTab();
        legend.printGameStatistic(player1);
        maze.displayPlayer(player1.y, player1.x, PLAYER1);
        sem_wait(synchroSem); //czeka na aktualiczacje ruchu wszystkich obiektow
        player1.printPlayerView();
        refresh();
    }

    closeConnection();
    //getch();
    endwin();
    return 0;
}
