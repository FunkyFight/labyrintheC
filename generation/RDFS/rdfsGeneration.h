#ifndef RDFS_GENERATION_H
#define RDFS_GENERATION_H

#include "../nodeListTool.h"
#include "../../business/nodes/labyrinthe_node.h"

int randomTravelCost();

struct ListNode* rdfsGeneration(struct LabyrintheNode* nodeStart, int height, int width);

int roadExists(struct ListNode* chemins,int x,int y);

struct ListNode* FillWithWalls(ListNode* chemins, int height, int width, int isPerfect);

struct ListNode* fullFillLabyrintheGeneration(int height,int width,int isPerfect, struct LabyrintheNode* starter);

#endif
