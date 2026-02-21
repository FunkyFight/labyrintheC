//
// Created by alex1 on 19/02/2026.
//

#include "PrimGeneration.h"

#include <stdlib.h>

#include "../nodeListTool.h"
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
struct ListNode* PrimGeneration(struct LabyrintheNode* nodeStart, int height, int width) {
    if (!isInLaby(nodeStart,height,width)) return NULL;
    struct ListNode* roadList = newListNode(10);
    struct ListNode* frontierList = newListNode(10);

    struct LabyrintheNode* LastVisitedNode = nodeStart;
    addToListNode(roadList, LastVisitedNode);
    addToListNode(frontierList, LastVisitedNode);

        for (int i = 0; i < 4; i++) {
            int nx = dirs[i][0];
            int ny = dirs[i][1];
            int exists = 0;
            for (int j = 0; j < roadList->size; j++) {
                if (roadList->nodeTab[j]->x == nx && roadList->nodeTab[j]->y == ny) {
                    struct LabyrintheNode* neighbor = LabyrintheNode_CreateCoords(nx, ny, randomTravelCost());
                    addToListNode(roadList, LastVisitedNode);
                    addToListNode(frontierList, neighbor);
                    inMaze[nx][ny] = 2;
                }
            }
        }

        while (frontierList->size > 0) {
            int dirs[4][2] = {
                {LastVisitedNode->x + 1, LastVisitedNode->y},
                {LastVisitedNode->x - 1, LastVisitedNode->y},
                {LastVisitedNode->x, LastVisitedNode->y + 1},
                {LastVisitedNode->x, LastVisitedNode->y - 1}
            };
            int choice = rand() % frontierList->size;
            LastVisitedNode = frontierList->nodeTab[choice];
            frontierList->nodeTab[choice] = frontierList->nodeTab[frontierList->size - 1];
            frontierList->size--;
            free(LastVisitedNode);
            inMaze[current->x][current->y] = 1;
            addToListNode(roadList, current);

            for (int i = 0; i < 4; i++) {
                int nx = LastVisitedNode->x + dirs[i][0];
                int ny = LastVisitedNode->y + dirs[i][1];
                if (nx >= 0 && ny >= 0 && nx < height && ny < width && inMaze[nx][ny] == 0) {
                    struct LabyrintheNode* neighbor = LabyrintheNode_CreateCoords(nx, ny, randomTravelCost());
                    addToListNode(frontierList, neighbor);
                    inMaze[nx][ny] = 2;
                }
            }
        }

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

struct ListNode* fullFillLabyrintheGeneration(int height,int width,int isPerfect, struct LabyrintheNode* starter) {
    srand(time(NULL));
    if (!starter) {
        starter = LabyrintheNode_CreateCoords(0,0,randomTravelCost());
    }
    starter->type = START;
    struct ListNode* chemins = rdfsGeneration(starter,height,width);

    struct ListNode* wallList = FillWithWalls(chemins,height,width,isPerfect);
    for (int i = 0; i < wallList->size; i++) {
        addToListNode(chemins, wallList->nodeTab[i]);
    }

    int* endXY = malloc(2 * sizeof(int));
    if (!endXY) {return NULL; }

    if (starter->x < width / 2) {
        if (starter->y < height / 2) {
            endXY[0] = width - 1;
            endXY[1] = height - 1;
        } else {
            endXY[0] = width - 1;
            endXY[1] = 0;
        }
    } else {
        if (starter->y < height / 2) {
            endXY[0] = 0;
            endXY[1] = height - 1;
        } else {
            endXY[0] = 0;
            endXY[1] = 0;
        }
    }

struct ListNode* fullFillLabyrintheGeneration(int height,int width,int isPerfect, struct LabyrintheNode* starter) {
    srand(time(NULL));
    if (!starter) {
        starter = LabyrintheNode_CreateCoords(0,0,randomTravelCost());
    }
    starter->type = START;
    struct ListNode* chemins = PrimGeneration(starter,height,width);

    struct ListNode* wallList = FillWithWalls(chemins,height,width,isPerfect);
    for (int i = 0; i < wallList->size; i++) {
        addToListNode(chemins, wallList->nodeTab[i]);
    }

    int* endXY = malloc(2 * sizeof(int));
    if (!endXY) {return NULL; }

    if (starter->x < width / 2) {
        if (starter->y < height / 2) {
            endXY[0] = width - 1;
            endXY[1] = height - 1;
        } else {
            endXY[0] = width - 1;
            endXY[1] = 0;
        }
    } else {
        if (starter->y < height / 2) {
            endXY[0] = 0;
            endXY[1] = height - 1;
        } else {
            endXY[0] = 0;
            endXY[1] = 0;
        }
    }

    struct LabyrintheNode* end = chemins->nodeTab[idLabyrintheNodeInListNode(chemins,endXY[0],endXY[1] )];
    end->type = END;
    if (end->travel_cost == 9999) {
        end->travel_cost = randomTravelCost();
    }
    free(endXY);

    return chemins;
}