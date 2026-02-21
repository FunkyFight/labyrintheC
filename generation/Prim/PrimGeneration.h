//
// Created by alex1 on 19/02/2026.
//

#ifndef LABYRINTHEC_PRIMGENERATION_H
#define LABYRINTHEC_PRIMGENERATION_H

#include "../nodeListTool.h"
#include "../../business/nodes/labyrinthe_node.h"

int randomTravelCost();

struct ListNode* PrimGeneration(struct LabyrintheNode* nodeStart, int height, int width);

int roadExists(struct ListNode* chemins,int x,int y);

struct ListNode* FillWithWalls(ListNode* chemins, int height, int width, int isPerfect);

struct ListNode* fullFillLabyrintheGeneration(int height,int width,int isPerfect, struct LabyrintheNode* starter);

#endif //LABYRINTHEC_PRIMGENERATION_H
