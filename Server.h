#ifndef MULTITHREADGAME_SERVER_H
#define MULTITHREADGAME_SERVER_H

#include "Player.h"
#include "Legend.h"
#include <semaphore.h>

#define FREE_SLOT 1
#define BUSY_SLOT 0

class Server {
private:

    WINDOW* win;
    int height;
    int width;

    Player player1;
    Player player2;

    int slot1Status = FREE_SLOT;
    int slot2Status = FREE_SLOT;

    Maze maze;
    int tileType;

    void setMazeOnServer(Maze maze);
    Legend legend;

    static void setColors();

    //Uchwyt na pamięć współdzielona
    int fd;
    struct shared_data_t *sharedData;

    //sem_t *roundTime;

public:
    void setClient(Player player);
    int serverInit();
    void memInit();
    void closeConnection();
    void ncursesInit();
};



#endif //MULTITHREADGAME_SERVER_H
