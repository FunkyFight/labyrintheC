#include <stdlib.h>

#include "nodeListTool.h"
#include "../../types.h"
#include "../../business/nodes/labyrinthe_node.h"


//50% DE CHANCES DE RENVOYER 1, 33% DE CHANCES DE RENVOYER 2 ET 17% DE CHANCES DE RENVOYER 3
int randomTravelCost() {
    srand(time(NULL));
    int r = rand() % 6;
    if (r < 3)
        return 1;
    if (r < 5)
        return 2;
    return 3;
}



//renvoie la liste des cases du chemins
ListNode* rdfsGeneration(LabyrintheNode* nodeStart, int height, int width) {
    if (!isInLaby(nodeStart,height,width)) return NULL;
    ListNode* roadList = newListNode(10);
    ListNode* roadInTakeList = newListNode(10);
    srand(time(NULL));

    LabyrintheNode* LastVisitedNode = nodeStart;
    addToListNode(roadList, LastVisitedNode);
    addToListNode(roadInTakeList, LastVisitedNode);

    while ((LastVisitedNode->x != 0 || LastVisitedNode -> y != 0) || roadInTakeList->size > 0){
        ListNode* possibilities = newListNode(4);

        LabyrintheNode* Neighbors[4] ;
        if (!LastVisitedNode->east) {
            LastVisitedNode->east = LabyrintheNode_CreateCoords(LastVisitedNode->x + 1, LastVisitedNode->y,randomTravelCost());
        }
        if (!LastVisitedNode->west) {
            LastVisitedNode->west = LabyrintheNode_CreateCoords(LastVisitedNode->x - 1, LastVisitedNode->y,randomTravelCost());
        }
        if (!LastVisitedNode->south) {
            LastVisitedNode->south = LabyrintheNode_CreateCoords(LastVisitedNode->x, LastVisitedNode->y + 1,randomTravelCost());
        }
        if (!LastVisitedNode->north) {
            LastVisitedNode->north = LabyrintheNode_CreateCoords(LastVisitedNode->x, LastVisitedNode->y - 1,randomTravelCost());
        }
        Neighbors[0] = LastVisitedNode->east;
        Neighbors[1] = LastVisitedNode->west;
        Neighbors[2] = LastVisitedNode->south;
        Neighbors[3] = LastVisitedNode->north;
        for (int i = 0; i < 4; i++) {
            LabyrintheNode* n = Neighbors[i];
            if (n && !containLabyrintheNode(roadList,n)&& !isInNeighborsList(roadList,n) && isInLaby(n,height ,width)) {
                addToListNode(possibilities,n);
            }
        }
        if (possibilities->size == 0) {
            removeLastToListNode(roadInTakeList);
            if (roadInTakeList->size > 0) {
                LastVisitedNode = roadInTakeList->nodeTab[roadInTakeList->size-1];
            }
        }else {
            int choise = rand()% possibilities->size;
            LastVisitedNode = possibilities->nodeTab[choise];
            addToListNode(roadList,LastVisitedNode);
            addToListNode(roadInTakeList,LastVisitedNode);
        }
        freeListNode(possibilities);
    }

    freeListNode(roadInTakeList);
    return roadList;
}

int roadExists(ListNode* chemins,int x,int y) {
    for (int i = 0; i < chemins->size; i++) {
        if (chemins->nodeTab[i]->x == x && chemins->nodeTab[i]->y == y) return 1;
    }
    return 0;
}

ListNode* FillWithWalls(ListNode* chemins, int height, int width, int isPerfect) {
    ListNode* wallList = newListNode(10);
    srand(time(NULL));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!roadExists(chemins,i,j)) {
                if (!isPerfect && (rand() % 9 == 0)) {
                    addToListNode(wallList,LabyrintheNode_CreateCoords(j,i,randomTravelCost()));
                }else {
                    addToListNode(wallList,LabyrintheNode_CreateCoords(j,i,9999));
                }
            }
        }
    }
    return wallList;
}

ListNode* fullFillLabyrintheGeneration(int height,int width,int isPerfect) {
    ListNode* chemins = rdfsGeneration(LabyrintheNode_CreateCoords(0,0,randomTravelCost()),height,width);
    ListNode* wallList = FillWithWalls(chemins,height,width,isPerfect);
    for (int i = 0; i < wallList->size; i++) {
        addToListNode(chemins, wallList->nodeTab[i]);
    }
    return chemins;
}
