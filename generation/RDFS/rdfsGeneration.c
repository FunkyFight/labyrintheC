#include <stdio.h>
#include <stdlib.h>

#include "../nodeListTool.h"
#include "../../types.h"
#include "../../business/nodes/labyrinthe_node.h"
#include "../../business/generationSteps/GenerationSteps.h"


//50% DE CHANCES DE RENVOYER 1, 33% DE CHANCES DE RENVOYER 2 ET 17% DE CHANCES DE RENVOYER 3
int randomTravelCost() {
    int r = rand() % 6;
    if (r < 3)
        return 1;
    if (r < 5)
        return 2;
    return 3;
}

//renvoie la liste des cases du chemins
struct ListNode* rdfsGeneration(struct LabyrintheNode* nodeStart, int height, int width, struct GenerationStep** steps, int* stepIndex) {
    if (!isInLaby(nodeStart,height,width)) return NULL;
    struct ListNode* roadList = newListNode(10);
    struct ListNode* roadInTakeList = newListNode(10);

    struct LabyrintheNode* LastVisitedNode = nodeStart;
    addToListNode(roadList, LastVisitedNode);
    addToListNode(roadInTakeList, LastVisitedNode);

    int iteration = 0;
    SDL_Color visitingColor = {100, 150, 255, 255}; // Bleu pour le noeud en cours de visite
    SDL_Color visitedColor = {50, 255, 50, 255};    // Vert pour les noeuds visités
    SDL_Color backtrackColor = {255, 100, 100, 255}; // Rouge pour le backtrack

    // Enregistrer le noeud de départ
    if (steps && stepIndex) {
        steps[(*stepIndex)++] = GenerationSteps_CreateSetNodeVisibilityStep(nodeStart->x, nodeStart->y, true, iteration);
        steps[(*stepIndex)++] = GenerationSteps_CreateHighlightExistingStepStep(nodeStart->x, nodeStart->y, visitingColor, iteration);
    }

    while (roadInTakeList->size > 0) {
        iteration++;
        ListNode* possibilities = newListNode(4);
        int dirs[4][2] = {
            {LastVisitedNode->x + 1, LastVisitedNode->y},
            {LastVisitedNode->x - 1, LastVisitedNode->y},
            {LastVisitedNode->x, LastVisitedNode->y + 1},
            {LastVisitedNode->x, LastVisitedNode->y - 1}
        };

        for (int i = 0; i < 4; i++) {
            int nx = dirs[i][0];
            int ny = dirs[i][1];
            int exists = 0;
            for (int j = 0; j < roadList->size; j++) {
                if (roadList->nodeTab[j]->x == nx && roadList->nodeTab[j]->y == ny) {
                    exists = 1;
                    break;
                }
            }
            if (exists || nx < 0 || ny < 0 || nx >= height || ny >= width) continue;

            struct LabyrintheNode* temp = LabyrintheNode_CreateCoords(nx, ny, randomTravelCost());
            int neighborCount = 0;
            struct LabyrintheNode* neighbors[4] = {
                LabyrintheNode_CreateCoords(nx+1, ny, 0),
                LabyrintheNode_CreateCoords(nx-1, ny, 0),
                LabyrintheNode_CreateCoords(nx, ny+1, 0),
                LabyrintheNode_CreateCoords(nx, ny-1, 0)
            };
            for (int k = 0; k < 4; k++) {
                for (int j = 0; j < roadList->size; j++) {
                    if (roadList->nodeTab[j]->x == neighbors[k]->x && roadList->nodeTab[j]->y == neighbors[k]->y) {
                        neighborCount++;
                    }
                }
                free(neighbors[k]);
            }
            if (neighborCount == 1) addToListNode(possibilities, temp);
            else free(temp);
        }

        if (possibilities->size == 0) {
            // Backtrack - colorer en rouge
            if (steps && stepIndex) {
                steps[(*stepIndex)++] = GenerationSteps_CreateHighlightExistingStepStep(LastVisitedNode->x, LastVisitedNode->y, backtrackColor, iteration);
            }
            removeLastToListNode(roadInTakeList);
            if (roadInTakeList->size > 0) LastVisitedNode = roadInTakeList->nodeTab[roadInTakeList->size-1];
        } else {
            int choise = rand() % possibilities->size;
            LastVisitedNode = possibilities->nodeTab[choise];

            if (LastVisitedNode) {
                // Enregistrer la création du nouveau noeud
                if (steps && stepIndex) {
                    steps[(*stepIndex)++] = GenerationSteps_CreateSetNodeVisibilityStep(LastVisitedNode->x,
                                                                                        LastVisitedNode->y, true,
                                                                                        iteration);
                    steps[(*stepIndex)++] = GenerationSteps_CreateHighlightExistingStepStep(LastVisitedNode->x,
                                                                                            LastVisitedNode->y,
                                                                                            visitingColor, iteration);
                }

                addToListNode(roadList, LastVisitedNode);
                addToListNode(roadInTakeList, LastVisitedNode);

                // Marquer l'ancien noeud comme visité (vert)
                if (steps && stepIndex && roadInTakeList->size > 1) {
                    struct LabyrintheNode *previousNode = roadInTakeList->nodeTab[roadInTakeList->size - 2];
                    steps[(*stepIndex)++] = GenerationSteps_CreateHighlightExistingStepStep(previousNode->x,
                                                                                            previousNode->y,
                                                                                            visitedColor, iteration);
                }
            }
        }
        freeListNode(possibilities);
    }

    freeListNode(roadInTakeList);
    return roadList;
}


int roadExists(ListNode* chemins,int x,int y) {
    for (int i = 0; i < chemins->size; i++) {
        if (chemins->nodeTab[i]->x == x && chemins->nodeTab[i]->y == y) return 1;
    }
    return 0;
}

struct ListNode* FillWithWalls(ListNode* chemins, int height, int width, int isPerfect, struct GenerationStep** steps, int* stepIndex) {
    struct ListNode* wallList = newListNode(10);
    int iteration = stepIndex ? *stepIndex : 0;
    SDL_Color wallColor = {80, 80, 80, 255}; // Gris pour les murs
    SDL_Color imperfectWallColor = {150, 150, 80, 255}; // Jaune-gris pour les murs imparfaits

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!roadExists(chemins,i,j)) {
                if (!isPerfect && (rand() % 9 == 0)) {
                    struct LabyrintheNode* wall = LabyrintheNode_CreateCoords(i,j,randomTravelCost());
                    addToListNode(wallList,wall);

                    // Enregistrer le mur imparfait (passage)
                    if (steps && stepIndex) {
                        steps[(*stepIndex)++] = GenerationSteps_CreateSetNodeVisibilityStep(i, j, true, iteration);
                        steps[(*stepIndex)++] = GenerationSteps_CreateHighlightExistingStepStep(i, j, imperfectWallColor, iteration);
                    }
                }else {
                    struct LabyrintheNode* wall = LabyrintheNode_CreateCoords(i,j,9999);
                    wall->type = WALL;
                    addToListNode(wallList,wall);

                    // Enregistrer le mur
                    if (steps && stepIndex) {
                        steps[(*stepIndex)++] = GenerationSteps_CreateSetNodeVisibilityStep(i, j, true, iteration);
                        steps[(*stepIndex)++] = GenerationSteps_CreateHighlightExistingStepStep(i, j, wallColor, iteration);
                    }
                }
            }
        }
    }
    return wallList;
}

struct ListNode* fullFillLabyrintheGeneration(int height,int width,int isPerfect, struct LabyrintheNode* starter, struct GenerationStep** steps) {
    srand(time(NULL));
    int stepIndex = 0;

    if (!starter) {
        starter = LabyrintheNode_CreateCoords(0,0,randomTravelCost());
    }
    starter->type = START;

    // Couleurs pour START et END
    SDL_Color startColor = {0, 255, 0, 255};  // Vert vif pour START
    SDL_Color endColor = {255, 0, 0, 255};    // Rouge vif pour END

    // Enregistrer le noeud START
    if (steps) {
        steps[stepIndex++] = GenerationSteps_CreateSetNodeVisibilityStep(starter->x, starter->y, true, 0);
        steps[stepIndex++] = GenerationSteps_CreateHighlightExistingStepStep(starter->x, starter->y, startColor, 0);
    }

    struct ListNode* chemins = rdfsGeneration(starter, height, width, steps, steps ? &stepIndex : NULL);

    struct ListNode* wallList = FillWithWalls(chemins, height, width, isPerfect, steps, steps ? &stepIndex : NULL);
    for (int i = 0; i < wallList->size; i++) {
        addToListNode(chemins, wallList->nodeTab[i]);
    }

    int* endXY = malloc(2 * sizeof(int));
    if (!endXY) {return NULL; }

    if (starter->x < width / 2) {
        if (starter->y < height / 2) {
            endXY[0] = width - 1;
            endXY[1] = height - 1;
        } else {
            endXY[0] = width - 1;
            endXY[1] = 0;
        }
    } else {
        if (starter->y < height / 2) {
            endXY[0] = 0;
            endXY[1] = height - 1;
        } else {
            endXY[0] = 0;
            endXY[1] = 0;
        }
    }

    struct LabyrintheNode* end = chemins->nodeTab[idLabyrintheNodeInListNode(chemins,endXY[0],endXY[1] )];
    end->type = END;
    if (end->travel_cost == 9999) {
        end->travel_cost = randomTravelCost();
    }

    // Enregistrer le noeud END
    if (steps) {
        steps[stepIndex++] = GenerationSteps_CreateHighlightExistingStepStep(end->x, end->y, endColor, stepIndex);

        // Ajouter la step End pour marquer la fin
        struct GenerationStep* endStep = malloc(sizeof(struct GenerationStep));
        endStep->stepType = End;
        endStep->step = NULL;
        endStep->iteration = stepIndex;
        steps[stepIndex] = endStep;
    }

    free(endXY);

    addToListNode(chemins,NULL);
    return chemins;
}

/**
 * Méthode prenable par Game. Doit toujours etre présente et etre sous cette signature dans un algo de génération
 * @param height
 * @param width
 * @param starter
 * @param steps
 * @return
 */
struct ListNode* make_rdfs_labyrinthe(int height,int width, struct LabyrintheNode* starter, struct GenerationStep** steps) {
    return fullFillLabyrintheGeneration(height, width, 1, starter, steps);
}
