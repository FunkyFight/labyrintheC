//
// Created by Funky on 30/01/2026.
//

#ifndef LABYRINTHE_GAMEFACADE_H
#define LABYRINTHE_GAMEFACADE_H

#include "../main.h"
#include "../gameobjects/gameobject_manager.h"
#include "../gameobjects/LabyrinthCell/gameobject_labyrinthe_cell.h"

void GameFacade_ShowInstantlyLabyrinthe(struct Labyrinthe* labyrinthe);
static void GameFacade_ShowNode(struct Game* game, struct LabyrintheNode* node, int cellSx, int cellY);
void DebugAndTest();

#endif //LABYRINTHE_GAMEFACADE_H
