#include <stdlib.h>

#include "nodeListTool.h"
#include "../../types.h"

ListNode* rdfsGeneration(LabyrintheNode* nodeStart) {
    ListNode* roadList = newListNode(10);
    ListNode* roadInTakeList = newListNode(10);
    srand(time(NULL));

    LabyrintheNode* LastVisitedNode = nodeStart;
    addToListNode(roadList, LastVisitedNode);
    addToListNode(roadInTakeList, LastVisitedNode);

    while ((LastVisitedNode->x != 0 || LastVisitedNode -> y != 0) || roadInTakeList->size > 0){
        ListNode* possibilities = newListNode(4);

        LabyrintheNode* Neighbors[4] ;
        Neighbors[0] = LastVisitedNode->east;
        Neighbors[1] = LastVisitedNode->west;
        Neighbors[2] = LastVisitedNode->south;
        Neighbors[3] = LastVisitedNode->north;
        for (int i = 0; i < 4; i++) {
            LabyrintheNode* n = Neighbors[i];
            if (n && !containLabyrintheNode(roadList,n)&& !isInNeighborsList(roadList,n)) {
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
