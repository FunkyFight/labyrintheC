//
// Created by rapha on 18/02/2026.
//

#include "pledge_solver.h"

#include <stdio.h>

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

boolean getCurrentNodeLeft(struct LabyrintheNode* currentNode,int direction)
{
    switch(direction)
    {
        case 0:
         if (currentNode->west->type == WALL)
         {
             return true;
         }
        case 1:
            if (currentNode->north->type == WALL)
            {
                return true;
            }
        case 2:
            if (currentNode->east->type == WALL)
            {
                return true;
            }
        case 3:
            if (currentNode->south->type == WALL)
            {
                return true;
            }
        default:
            return false;

    }
}

boolean getWallInFront(struct LabyrintheNode* currentNode,int direction)
{
    if(getNodeInFront(currentNode,directiona)->type == WALL)
    {
        return true;
    }
    if(getNodeInFront(currentNode,direction)->type == CELL)
    {
        return false;
    }

}



void Pledge_Solver(struct PledgeSolverData* data )
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
    struct LabyrintheNode* nodeInFront = getNodeInFront(data->node,data->discision);

    if (data->discision == 0 && getWallInFront(data->node,data->discision) == false)
    {
        data->node = data->node->north;
        Pledge_Solver(data);
    }

    if (data->node->type != END)
    {
        // condition si mur devant et mais pas mur a gauche
        if (getCurrentNodeLeft(data->node,data->discision) == false && getWallInFront(data->node,data->discision) == true)
        {
            data->solverDirection = (data->solverDirection+1)%4;
            data->discision = data->discision-1;
            Pledge_Solver(data);
        }
        // condition si mur devant et mur a gauche
        if (getCurrentNodeLeft(data->node,data->discision) == true && getWallInFront(data->node,data->discision) == true)
        {
            // pour que ça reste entre 0 et 3 compris
            if (data->solverDirection == 0)
            {
                data->solverDirection = 3;
            }
            else
            {
                data->solverDirection = (data->solverDirection-1)%4;
            }
            data->discision = data->discision+1;
            Pledge_Solver(data);
        }
        // condition si pas mur devant et pas mur a gauche
        if (getCurrentNodeLeft(data->node,data->discision) == false && getWallInFront(data->node,data->discision) == false)
        {
            //je pourrai le mettre au dessus mais plus explicite comme ça
            // pour que ça reste entre 0 et 3 compris
            if (data->solverDirection == 0)
            {
                data->solverDirection = 3;
            }
            else
            {
                data->solverDirection = (data->solverDirection-1)%4;
            }
            data->discision = data->discision+1;
            Pledge_Solver(data);
        }
    }




    data->node = nodeInFront;
    addToListNode(data->pathSolved, nodeInFront);
    Pledge_Solver(data);
}