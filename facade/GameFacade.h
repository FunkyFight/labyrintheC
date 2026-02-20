//
// Created by Funky on 30/01/2026.
//

#ifndef LABYRINTHE_GAMEFACADE_H
#define LABYRINTHE_GAMEFACADE_H

#include "../main.h"
#include "../gameobjects/gameobject_manager.h"
#include "../gameobjects/LabyrinthCell/gameobject_labyrinthe_cell.h"

// Forward declarations
struct ListNode;
struct LabyrintheNode;
struct Labyrinthe;

void GameFacade_ShowInstantlyLabyrinthe(struct Labyrinthe* labyrinthe);
struct LabyrintheNode* GameFacade_Labyrinthe_Tab_To_Nodes(struct ListNode* listNodes);
struct ListNode* GameFacade_Labyrinthe_Nodes_To_Tab(struct LabyrintheNode* root);
void DebugAndTest();

#endif //LABYRINTHE_GAMEFACADE_H
