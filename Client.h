//
// Created by root on 2/20/21.
//

#ifndef MULTITHREADGAME_CLIENT_H
#define MULTITHREADGAME_CLIENT_H

#include "Player.h"
#include "Legend.h"

class Client{

    WINDOW* win;
    int height, width;

    Legend legend;

    struct timespec tm;

    //Uchwyt na pamięć współdzielona
    int fd;
    struct shared_data_t *sharedData;

    sem_t *validateMoveSem; //przekazuje info ze ruch jest poprawny
    sem_t *waitForClient; // server czeka na ruch klienta
    sem_t *roundTime; //inicjuje runde
    sem_t *dataSet; //ustawianie danych poczatkowych
    sem_t *newClientAsk; //klient wyslal dane
    sem_t *synchroSem; //czeka na aktualiczacje ruchu wszystkich obiektow

public:

    void closeConnection();
    int clientJoinToServer();
    int runClient(void);
    void setColors();
    int valicateMove(int y, int x);
    int serverDosntExist();
    void ncursesInit();

    static void *displayMap(void *args);
};
#endif //MULTITHREADGAME_CLIENT_H
