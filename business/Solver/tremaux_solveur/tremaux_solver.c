//
// Created by rapha on 22/02/2026.
//

#include "tremaux_solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../../types.h"


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




void Right_Solver(struct RightSolverData* data )
{
    if (data->node->type != START && data->isPathSolvedListNodeCreated == false)
    {
        fprintf(stderr, "Erreur: type de node inattendu (%d)\n", node->type);
        exit(EXIT_FAILURE);
    }
    if (!data->isPathSolvedListNodeCreated)
    {
        data->pathSolver = newListNode (data->pathSolver, 1);
        data->isPathSolvedListNodeCreated = true;
    }
    if  (data->node->type == END)
    {
        printf("GG t'as terminier le labytrinth\n");
        return;
    }
    struct LabyrintheNode* nodeInFront = getNodeInFront(data->node,data->solverDirection);
    if (nodeInFront->type == WALL)
    {
        data->solverDirection = (data->solverDirection+1)%4;
        //addToListNode(data->pathSolved,);
        Right_Solver(data);
        return;
    }

    //tout ce qui ce passe en bas sont sur les cellules
    data->node = nodeInFront;
    addToListNode(data->pathSolved, nodeInFront);
    Right_Solver(data);

}