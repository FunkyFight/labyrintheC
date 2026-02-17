#include "nodeListTool.h"
#include <stdlib.h>

ListNode* newListNode(int startMaxSize) {
    ListNode* l = malloc(sizeof(ListNode));
    l->size = 0;
    l->maxSize = startMaxSize;
    l->nodeTab = malloc(sizeof(LabyrintheNode*) * startMaxSize);
    return l;
}




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

int isInNeighbors(LabyrintheNode* analysedNode, LabyrintheNode* n) {
    if (!analysedNode || !n) return 0;

    int dx = abs(analysedNode->x - n->x);
    int dy = abs(analysedNode->y - n->y);

    if (dx + dy == 1)
        return 1;

    return 0;
}

int isInNeighborsList(ListNode* l,LabyrintheNode* n) {
    for (int i = 0; i < l->size; i++) {
        if (isInNeighbors(l->nodeTab[i],n)) return 1;
    }
    return 0;
};

int isInLaby(LabyrintheNode* node,int height, int width) {
    if (node == NULL) return 0;
    if (node->x < height && node->y < width && node->x >= 0 && node->y >= 0) return 1;
}