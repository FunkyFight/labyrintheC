//
// Created by Funky on 30/01/2026.
//

#ifndef LABYRINTHE_MAIN_H
#define LABYRINTHE_MAIN_H

#include <stdbool.h>
#include <stdlib.h>

struct Game;

bool game_init_sdl(struct Game *g);
void game_free(struct Game *g);
void game_run(struct Game *g);

extern struct Game *currentGame;

#endif //LABYRINTHE_MAIN_H
