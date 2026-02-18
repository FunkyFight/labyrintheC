#ifndef LABYRINTHE_NODELISTTOOL_H
#define LABYRINTHE_NODELISTTOOL_H

#include "../types.h"
//Je suis obligé de le faire car tu n'a pas mis de nom de déclaration
struct LabyrintheNode;
typedef struct LabyrintheNode LabyrintheNode;
//

typedef struct ListNode {
    int size;
    int maxSize;
    LabyrintheNode** nodeTab;
}ListNode;

ListNode* newListNode(int startMaxSize);
void addToListNode(ListNode* l,LabyrintheNode* n);
LabyrintheNode* removeLastToListNode(ListNode* l);
int containLabyrintheNode(ListNode* l,LabyrintheNode* n);
void freeListNode(ListNode* l);
int isInLaby(LabyrintheNode* node,int height, int width);




#endif //LABYRINTHE_NODELISTTOOL_H