#ifndef RDFS_GENERATION_H
#define RDFS_GENERATION_H

#include "nodeListTool.h"
#include "../../business/nodes/labyrinthe_node.h"

int randomTravelCost();

ListNode* rdfsGeneration(LabyrintheNode* nodeStart, int height, int width);

int roadExists(ListNode* chemins,int x,int y);

ListNode* FillWithWalls(ListNode* chemins, int height, int width, int isPerfect);

ListNode* fullFillLabyrintheGeneration(int height,int width,int isPerfect);

#endif
