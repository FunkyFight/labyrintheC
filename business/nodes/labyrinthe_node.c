#include <stdlib.h>
#include <time.h>
#include "../../types.h"

struct LabyrintheNode* LabyrintheNode_Create() {
    struct LabyrintheNode* node;
    node = malloc(sizeof(struct LabyrintheNode));

    node->north = NULL;
    node->south = NULL;
    node->east = NULL;
    node->west = NULL;
    node->visited = false;

    return node;
}

void LabyrintheNode_Destroy(struct LabyrintheNode* node) {
    if(node->north != NULL) LabyrintheNode_Destroy(node->north);
    if(node->south != NULL) LabyrintheNode_Destroy(node->south);
    if(node->east != NULL) LabyrintheNode_Destroy(node->east);
    if(node->west != NULL) LabyrintheNode_Destroy(node->west);

    free(node);
}
