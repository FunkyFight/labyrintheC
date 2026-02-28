//
// Created by rapha on 26/02/2026.
//

#include "gauchedroite_solver.h"

#include  "gauchedroite_solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../../types.h"
#include "../../../generation/nodeListTool.h"


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

struct LabyrintheNode* getNodeInLeft(struct LabyrintheNode* currentNode, int direction) {
    switch(direction) {
    case 0:
        return currentNode->west;
    case 1:
        return currentNode->north;
    case 2:
        return currentNode->east;
    case 3:
        return currentNode->south;
    }

    return NULL;
}

struct LabyrintheNode* getNodeInRight(struct LabyrintheNode* currentNode, int direction) {
    switch(direction) {
    case 0:
        return currentNode->east;
    case 1:
        return currentNode->south;
    case 2:
        return currentNode->west;
    case 3:
        return currentNode->north;
    }

    return NULL;
}

int absAndDiscision(int direction)
{
    if (direction == -1)
    {
        return 3;
    }
    return direction;
}

void GaucheDroite_Solver(struct GaucheDroiteSolverData* data )
{
    if (data->node->type != START && data->isPathSolvedListNodeCreated == false)
    {
        fprintf(stderr, "Erreur: type de node inattendu (%d)\n", data->node->type);
        exit(EXIT_FAILURE);
    }
    if (!data->isPathSolvedListNodeCreated)
    {
        data->pathSolved = newListNode (1);
        data->isPathSolvedListNodeCreated = true;
    }
    if  (data->node->type == END)
    {
        printf("GG t'as terminier le labytrinth\n");
        return;
    }
    struct LabyrintheNode* nodeInFront = getNodeInFront(data->node,data->solverDirection);
    struct LabyrintheNode* nodeInLeft = getNodeInLeft(data->node,data->solverDirection);
    struct LabyrintheNode* nodeInRight = getNodeInRight(data->node,data->solverDirection);
    if (nodeInLeft->type == CELL && data->droiteGauche)
    {
        data->node = nodeInLeft;
        addToListNode(data->pathSolved, nodeInLeft);
        data->solverDirection = (data->solverDirection-1)%4;
        data->solverDirection = absAndDiscision(data->solverDirection);
        data->droiteGauche = false;
        GaucheDroite_Solver(data);

    }

    if (nodeInRight->type == CELL && !data->droiteGauche)
    {
        data->node = nodeInLeft;
        addToListNode(data->pathSolved, nodeInLeft);
        data->solverDirection = (data->solverDirection=+1)%4;
        data->solverDirection = absAndDiscision(data->solverDirection);
        data->droiteGauche = true;
        GaucheDroite_Solver(data);

    }

    if (nodeInLeft->type == CELL)
    {
        data->node = nodeInLeft;
        addToListNode(data->pathSolved, nodeInLeft);
        data->solverDirection = (data->solverDirection-1)%4;
        data->solverDirection = absAndDiscision(data->solverDirection);
        data->droiteGauche = !data->droiteGauche;
        GaucheDroite_Solver(data);

    }
    if (nodeInRight->type == CELL)
    {
        data->node = nodeInRight;
        addToListNode(data->pathSolved, nodeInRight);
        data->solverDirection = (data->solverDirection+1)%4;
        data->solverDirection = absAndDiscision(data->solverDirection);
        data->droiteGauche = !data->droiteGauche;
        GaucheDroite_Solver(data);

    }


    if (nodeInRight->type == WALL)
    {
        data->solverDirection = (data->solverDirection+1)%4;
        //addToListNode(data->pathSolved,);
        GaucheDroite_Solver(data);
        return;
    }



    //tout ce qui ce passe en bas sont sur les cellules
    data->node = nodeInFront;
    addToListNode(data->pathSolved, nodeInFront);
    GaucheDroite_Solver(data);

}


