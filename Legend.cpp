#include <ncurses.h>
#include "Legend.h"
#include "PlayerHandler.h"
#include "Maze.h"


void Legend::printLegend() {
    int lHeight = height * 2 / 3;
    attron(COLOR_PAIR(PLAYER_PAIR));
    mvprintw(lHeight, width, "12");
    attroff(COLOR_PAIR(EMPTY_PAIR));
    attron(COLOR_PAIR(WALL_PAIR));
    mvprintw(lHeight+1, width, " ");
    attroff(COLOR_PAIR(EMPTY_PAIR));
    mvprintw(lHeight+2, width, "#");
    attron(COLOR_PAIR(BEAST_PAIR));
    mvprintw(lHeight+3, width, "*");
    attroff(COLOR_PAIR(BEAST_PAIR));

    attron(COLOR_PAIR(COIN_PAIR));
    mvprintw(lHeight+4, width, "c");
    mvprintw(lHeight+5, width, "C");
    mvprintw(lHeight+6, width, "T");
    attroff(COLOR_PAIR(COIN_PAIR));

    mvprintw(lHeight, width + 3, "- players");
    mvprintw(lHeight+1, width + 3, "- wall");
    mvprintw(lHeight+2, width + 3, "- bushes (slow down)");
    mvprintw(lHeight+3, width + 3, "- beast (enemy)");
    mvprintw(lHeight+4, width + 3, "- one coin");
    mvprintw(lHeight+5, width + 3, "- small treasure (10 coins)");
    mvprintw(lHeight+6, width + 3, "- large treasure (50 coins)");
    refresh();
}

void Legend::setPosition(int width, int height) {
    Legend::width = width;
    Legend::height = height;
}

void Legend::printGameStatistic(PlayerHandler player) {
    mvprintw(height/2, width , "Player 1 carried points: %d  ", player.carriedCoins);
    mvprintw(height/2 +1, width , "Player 1 brought points: %d  ", player.broughtCoins);
    refresh();
}
