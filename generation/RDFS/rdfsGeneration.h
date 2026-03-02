#ifndef RDFS_GENERATION_H
#define RDFS_GENERATION_H

#include "../nodeListTool.h"
#include "../../business/nodes/labyrinthe_node.h"

int randomTravelCost();

struct ListNode* rdfsGeneration(struct LabyrintheNode* nodeStart, int height, int width, struct GenerationStep** steps, int* stepIndex);

int roadExists(struct ListNode* chemins,int x,int y);

struct ListNode* FillWithWalls(ListNode* chemins, int height, int width, int isPerfect, struct GenerationStep** steps, int* stepIndex);

struct ListNode* fullFillLabyrintheGeneration(int height,int width,int isPerfect, struct LabyrintheNode* starter, struct GenerationStep** steps);

struct ListNode* make_rdfs_labyrinthe(int height,int width, struct LabyrintheNode* starter, struct GenerationStep** steps);

#endif
