//
// Created by Funky on 16/12/2025.
//

#ifndef LABYRINTHE_LABYRINTHE_NODE_H
#define LABYRINTHE_LABYRINTHE_NODE_H

struct LabyrintheNode* LabyrintheNode_Create();
void LabyrintheNode_Destroy(struct LabyrintheNode* node);
struct LabyrintheNode* LabyrintheNode_CreateCoords(int x, int y, int travel_cost);

#endif //LABYRINTHE_LABYRINTHE_NODE_H
