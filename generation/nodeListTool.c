#include "nodeListTool.h"
#include <stdlib.h>

ListNode* newListNode(int startMaxSize) {
    ListNode* l = malloc(sizeof(ListNode));
    l->size = 0;
    l->maxSize = startMaxSize;
    l->nodeTab = malloc(sizeof(struct LabyrintheNode*) * startMaxSize);
    return l;
}




void addToListNode(ListNode* l, struct LabyrintheNode* n) {
    if (l->size >= l->maxSize) {
        l->maxSize ++;
        l->nodeTab = realloc(l->nodeTab, sizeof(struct LabyrintheNode*) * l->maxSize);
    }
    l->nodeTab[l->size] = n;
    l->size++;
};



struct LabyrintheNode* removeLastToListNode(ListNode* l) {
    if (l->size == 0) return NULL;
    struct LabyrintheNode* last = l->nodeTab[l->size - 1];
    l->size--;

    return last;
};



int containLabyrintheNode(ListNode* l, struct LabyrintheNode* n) {
    for (int i = 0; i < l->size; i++) {
        if (l->nodeTab[i] == n) return 1;
    }
    return 0;
};



void freeListNode(ListNode* l) {
    free(l->nodeTab);
    free(l);
};

int isInLaby(struct LabyrintheNode* node, int height, int width) {
    if (node == NULL) return 0;
    if (node->x >= 0 && node->x < width &&
        node->y >= 0 && node->y < height) return 1;
    return 0;
}

