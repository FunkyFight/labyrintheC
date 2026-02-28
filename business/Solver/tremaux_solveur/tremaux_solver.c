//
// Created by rapha on 22/02/2026.
//

#include "tremaux_solver.h"
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



boolean alreadyDiscovered(struct LabyrintheNode* currentNode,struct TremauxSolverData* data)
{
    if (data == NULL || data->pathAlreadyDiscovered == NULL) {
        return false;
    }

    return containLabyrintheNode(data->pathAlreadyDiscovered, currentNode) ? true : false;
}

boolean pathImpath(struct LabyrintheNode* currentNode,struct TremauxSolverData* data)
{
    if (data == NULL || data->pathImpath == NULL) {
        return false;
    }

    return containLabyrintheNode(data->pathImpath, currentNode) ? true : false;
}

int isMark0(struct TremauxSolverData* data, struct LabyrintheNode* n) {
    if (n == NULL || n->type == WALL) return 0;
    if (containLabyrintheNode(data->pathImpath, n)) return 0;
    if (containLabyrintheNode(data->pathAlreadyDiscovered, n)) return 0;
    return 1;
}

int isMark1(struct TremauxSolverData* data, struct LabyrintheNode* n) {
    if (n == NULL || n->type == WALL) return 0;
    if (containLabyrintheNode(data->pathImpath, n)) return 0;
    if (!containLabyrintheNode(data->pathAlreadyDiscovered, n)) return 0;
    return 1;
}


struct LabyrintheNode* chooseNextNode(struct TremauxSolverData* data, struct LabyrintheNode* current, int* chosenDir)
{
    int facing = data->solverDirection;

    int dLeft  = (facing + 3) % 4;
    int dFront = facing;
    int dRight = (facing + 1) % 4;
    int dBack  = (facing + 2) % 4;

    struct LabyrintheNode* left  = getNodeInFront(current, dLeft);
    struct LabyrintheNode* front = getNodeInFront(current, dFront);
    struct LabyrintheNode* right = getNodeInFront(current, dRight);
    struct LabyrintheNode* back  = getNodeInFront(current, dBack);

    if (isMark0(data, left))  { *chosenDir = dLeft;  return left;  }
    if (isMark0(data, front)) { *chosenDir = dFront; return front; }
    if (isMark0(data, right)) { *chosenDir = dRight; return right; }
    if (isMark0(data, back))  { *chosenDir = dBack;  return back;  }

    if (isMark1(data, left))  { *chosenDir = dLeft;  return left;  }
    if (isMark1(data, front)) { *chosenDir = dFront; return front; }
    if (isMark1(data, right)) { *chosenDir = dRight; return right; }
    if (isMark1(data, back))  { *chosenDir = dBack;  return back;  }

    *chosenDir = -1;
    return NULL;
}



int removeNodeFromList(ListNode* l, struct LabyrintheNode* n) {
    if (l == NULL || n == NULL) return 0;
    for (int i = 0; i < l->size; i++) {
        if (l->nodeTab[i] == n) {
            l->nodeTab[i] = l->nodeTab[l->size - 1];
            l->size--;
            return 1;
        }
    }
    return 0;
}

void setMark2(struct TremauxSolverData* data, struct LabyrintheNode* node)
{
    if (!containLabyrintheNode(data->pathImpath, node)) {
        addToListNode(data->pathImpath, node);
    }
    removeNodeFromList(data->pathAlreadyDiscovered, node);
}

void backtrackStep(struct TremauxSolverData* data)
{
    struct LabyrintheNode* nodeActuell = removeLastToListNode(data->pathBuffer);
    if (nodeActuell == NULL || data->pathBuffer->size == 0) {
        data->impasse = true;
        return;
    }
    setMark2(data, nodeActuell);
    // nouveau node on prend le dernier de la pil du buffer
    data->node = data->pathBuffer->nodeTab[data->pathBuffer->size - 1];
}



void Tremaux_Solver(struct TremauxSolverData* data )
{
    if (data->node->type != START && data->isPathSolvedListNodeCreated == false)
    {
        fprintf(stderr, "Erreur: type de node inattendu (%d)\n", data->node->type);
        exit(EXIT_FAILURE);
    }
    if (!data->isPathSolvedListNodeCreated)
    {
        data->pathSolved = newListNode (1);
        data->pathAlreadyDiscovered = newListNode (1);
        data->pathImpath = newListNode (1);
        data->pathBuffer = newListNode(1);
        data->isPathSolvedListNodeCreated = true;
        data->impasse = false;
        data->compteur = 0;
        addToListNode(data->pathSolved, data->node);
        addToListNode(data->pathBuffer, data->node);
    }

    struct LabyrintheNode* nodeActuell = data->node;
    if  (nodeActuell->type == END)
    {
        printf("GG t'as terminier le labytrinth\n");
        return;
    }

    int chosenDir = -1;
    struct LabyrintheNode* choice = chooseNextNode(data, nodeActuell, &chosenDir);

    // si il y a un choix qui a été fait
    if (choice != NULL) {

        data->solverDirection = chosenDir;
        data->node = choice;

        if (!containLabyrintheNode(data->pathAlreadyDiscovered, choice) && !containLabyrintheNode(data->pathImpath, choice))
        {
            addToListNode(data->pathAlreadyDiscovered, choice);
        }

        addToListNode(data->pathBuffer, choice);
        addToListNode(data->pathSolved, choice);

        data->compteur++;
        Tremaux_Solver(data);
    }


    // sinon on fait on revien en arrière :
    if (!containLabyrintheNode(data->pathImpath, nodeActuell)) {
        addToListNode(data->pathImpath, nodeActuell);
    }


    removeLastToListNode(data->pathBuffer);

    if (data->pathBuffer->size == 0) {
        data->impasse = true;
        return;
    }


    backtrackStep(data);
    addToListNode(data->pathSolved, data->node);
    data->compteur++;
    Tremaux_Solver(data);
}