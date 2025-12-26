#include "nodeListTool.h"
#include <stdlib.h>

ListNode* newListNode(int startMaxSize) {
    ListNode* l = (ListNode*)malloc(sizeof(ListNode));
    l->size = 0;
    l->maxSize = startMaxSize;
    l->nodeTab = realloc(l->nodeTab, sizeof(ListNode) * startMaxSize);
    return l;


};



void addToListNode(ListNode* l, LabyrintheNode* n) {
    if (l->size >= l->maxSize) {
        l->maxSize ++;
        l->nodeTab = realloc(l->nodeTab, sizeof(LabyrintheNode*) * l->maxSize);
    }
    l->nodeTab[l->size] = n;
    l->size++;
};



LabyrintheNode* removeLastToListNode(ListNode* l) {
    if (l->size == 0) return NULL;
    LabyrintheNode* last = l->nodeTab[l->size - 1];
    l->size--;

    return last;
};



int containLabyrintheNode(ListNode* l, LabyrintheNode* n) {
    for (int i = 0; i < l->size; i++) {
        if (l->nodeTab[i] == n) return 1;
    }
    return 0;
};



void freeListNode(ListNode* l) {
    free(l->nodeTab);
    free(l);
};

int isInNeighbors(LabyrintheNode* analysedNode,LabyrintheNode* n) {
    LabyrintheNode* Neighbors[4] ;
    Neighbors[0] = analysedNode->east;
    Neighbors[1] = analysedNode->west;
    Neighbors[2] = analysedNode->south;
    Neighbors[3] = analysedNode->north;
    for (int i = 0; i < 4; i++) {
        LabyrintheNode* nghbr = Neighbors[i];
        if (nghbr && nghbr == n) {
            return 1;
        }
    }
    return 0;
};
int isInNeighborsList(ListNode* l,LabyrintheNode* n) {
    for (int i = 0; i < l->size; i++) {
        if (isInNeighbors(l->nodeTab[i],n)) return 1;
    }
    return 0;
};