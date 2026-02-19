//
// Created by Funky on 19/02/2026.
//

#include "node_sort.h"
#include "../types.h"


int NodeSort_compareY(const void *a, const void *b)
{
    struct LabyrintheNode* nodeA = (struct LabyrintheNode*)a;
    struct LabyrintheNode* nodeB = (struct LabyrintheNode*)b;

    if(nodeA->y < nodeB->y) return -1;
    if(nodeA->y > nodeB->y) return 1;
    return 0;
}