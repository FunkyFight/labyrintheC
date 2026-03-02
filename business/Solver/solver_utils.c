#include "solver_utils.h"

struct LabyrintheNode* getNodeInFront(struct LabyrintheNode* currentNode, int direction) {
    switch(direction) {
    case 0:
        return currentNode->north;
    case 1:
        return currentNode->east;
    case 2:
        return currentNode->south;
    case 3:
        return currentNode->west;
    }

    return NULL;
}
