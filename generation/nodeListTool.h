#ifndef LABYRINTHE_NODELISTTOOL_H
#define LABYRINTHE_NODELISTTOOL_H

#include "../types.h"

typedef struct ListNode {
    int size;
    int maxSize;
    struct LabyrintheNode** nodeTab;
}ListNode;

ListNode* newListNode(int startMaxSize);
void addToListNode(struct ListNode* l, struct LabyrintheNode* n);
struct LabyrintheNode* removeLastToListNode(ListNode* l);
int containLabyrintheNode(struct ListNode* l, struct LabyrintheNode* n);
void freeListNode(struct ListNode* l);
int isInLaby(struct LabyrintheNode* node,int height, int width);




#endif //LABYRINTHE_NODELISTTOOL_H