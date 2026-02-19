#ifndef LABYRINTHE_NODELISTTOOL_H
#define LABYRINTHE_NODELISTTOOL_H

#include "../types.h"

typedef struct ListNode {
    int size;
    int maxSize;
    struct LabyrintheNode** nodeTab;
}ListNode;

ListNode* newListNode(int startMaxSize);
void addToListNode(ListNode* l, struct LabyrintheNode* n);
struct LabyrintheNode* removeLastToListNode(ListNode* l);
int containLabyrintheNode(ListNode* l, struct LabyrintheNode* n);
void freeListNode(ListNode* l);
int isInLaby(struct LabyrintheNode* node,int height, int width);




#endif //LABYRINTHE_NODELISTTOOL_H