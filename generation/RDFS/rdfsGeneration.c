#include <stdlib.h>

#include "nodeListTool.h"
#include "../../types.h"
#include "../../business/nodes/labyrinthe_node.h"


//50% DE CHANCES DE RENVOYER 1, 33% DE CHANCES DE RENVOYER 2 ET 17% DE CHANCES DE RENVOYER 3
int randomTravelCost() {
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

    LabyrintheNode* LastVisitedNode = nodeStart;
    addToListNode(roadList, LastVisitedNode);
    addToListNode(roadInTakeList, LastVisitedNode);

    while (roadInTakeList->size > 0) {
        ListNode* possibilities = newListNode(4);
        int dirs[4][2] = {
            {LastVisitedNode->x + 1, LastVisitedNode->y},
            {LastVisitedNode->x - 1, LastVisitedNode->y},
            {LastVisitedNode->x, LastVisitedNode->y + 1},
            {LastVisitedNode->x, LastVisitedNode->y - 1}
        };

        for (int i = 0; i < 4; i++) {
            int nx = dirs[i][0];
            int ny = dirs[i][1];
            int exists = 0;
            for (int j = 0; j < roadList->size; j++) {
                if (roadList->nodeTab[j]->x == nx && roadList->nodeTab[j]->y == ny) {
                    exists = 1;
                    break;
                }
            }
            if (exists || nx < 0 || ny < 0 || nx >= height || ny >= width) continue;

            LabyrintheNode* temp = LabyrintheNode_CreateCoords(nx, ny, randomTravelCost());
            int neighborCount = 0;
            LabyrintheNode* neighbors[4] = {
                LabyrintheNode_CreateCoords(nx+1, ny, 0),
                LabyrintheNode_CreateCoords(nx-1, ny, 0),
                LabyrintheNode_CreateCoords(nx, ny+1, 0),
                LabyrintheNode_CreateCoords(nx, ny-1, 0)
            };
            for (int k = 0; k < 4; k++) {
                for (int j = 0; j < roadList->size; j++) {
                    if (roadList->nodeTab[j]->x == neighbors[k]->x && roadList->nodeTab[j]->y == neighbors[k]->y) {
                        neighborCount++;
                    }
                }
                free(neighbors[k]);
            }
            if (neighborCount == 1) addToListNode(possibilities, temp);
            else free(temp);
        }

        if (possibilities->size == 0) {
            removeLastToListNode(roadInTakeList);
            if (roadInTakeList->size > 0) LastVisitedNode = roadInTakeList->nodeTab[roadInTakeList->size-1];
        } else {
            int choise = rand() % possibilities->size;
            LastVisitedNode = possibilities->nodeTab[choise];
            addToListNode(roadList, LastVisitedNode);
            addToListNode(roadInTakeList, LastVisitedNode);
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
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!roadExists(chemins,i,j)) {
                if (!isPerfect && (rand() % 9 == 0)) {
                    addToListNode(wallList,LabyrintheNode_CreateCoords(i,j,randomTravelCost()));
                }else {
                    addToListNode(wallList,LabyrintheNode_CreateCoords(i,j,9999));
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
