#include <stdlib.h>
#include <time.h>
#include "../../types.h"

void Labyrinthe_InitTest5x5(struct Labyrinthe *laby) {
    laby->width = 5;
    laby->height = 5;

    srand(time(NULL));

    struct LabyrintheNode* tempGrid[5][5];

    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            struct LabyrintheNode *node;

            if (x == 0 && y == 0) {
                node = &laby->firstNode;
            } else {
                node = malloc(sizeof(struct LabyrintheNode));
            }

            node->x = x;
            node->y = y;

            node->north = NULL;
            node->south = NULL;
            node->east = NULL;
            node->west = NULL;

            if (x == 0 && y == 0) {
                node->type = CELL;
                node->travel_cost = 0;
            } else {
                int r = rand() % 100;
                if (r < 70) {
                    node->type = CELL;
                    node->travel_cost = 1;
                } else {
                    node->type = WALL;
                    node->travel_cost = 9999;
                }
            }

            tempGrid[x][y] = node;
        }
    }

    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            struct LabyrintheNode *currentNode = tempGrid[x][y];

            if (y > 0) currentNode->north = tempGrid[x][y - 1];
            if (y < 4) currentNode->south = tempGrid[x][y + 1];
            if (x > 0) currentNode->west  = tempGrid[x - 1][y];
            if (x < 4) currentNode->east  = tempGrid[x + 1][y];
        }
    }
}