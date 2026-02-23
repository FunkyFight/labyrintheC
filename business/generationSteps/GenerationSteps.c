//
// Created by Funky on 19/02/2026.
//

#include <stdio.h>
#include "GenerationSteps.h"
#include "../../types.h"
#include "../../facade/GameFacade.h"
#include "../nodes/labyrinthe_node.h"
#include "../../generation/nodeListTool.h"
#include "../../utils/SFX.h"

#define INTERVAL_OF_ITERATION_FOR_EVERY_SFX 30

struct GenerationStep **GenerationSteps_Create()
{
    struct GenerationStep** strct = malloc(sizeof(struct GenerationStep*) * 9999);
    return strct;
}

void GenerationSteps_Free(struct GenerationStep** strct)
{
    free(strct);
}

struct GenerationStep *GenerationSteps_CreateSetNodeVisibilityStep(int x, int y, bool visible, int iteration)
{
    enum StepType stepType = SetNodeVisibility;

    struct SetNodeVisibilityStepType *step = malloc(sizeof(struct SetNodeVisibilityStepType));
    step->x = x;
    step->y = y;
    step->visible = visible;

    struct GenerationStep *generationStep = malloc(sizeof(struct GenerationStep));

    generationStep->step = step;
    generationStep->stepType = stepType;
    generationStep->iteration = iteration;

    return generationStep;
}

struct GenerationStep *GenerationSteps_CreateHighlightExistingStepStep(int x, int y, SDL_Color color, int iteration)
{
    enum StepType stepType = HighlightExistingNode;

    struct HighlightExistingStepType *step = malloc(sizeof(struct HighlightExistingStepType));
    step->x = x;
    step->y = y;
    step->color = color;

    struct GenerationStep *generationStep = malloc(sizeof(struct GenerationStep));

    generationStep->step = step;
    generationStep->stepType = stepType;
    generationStep->iteration = iteration;

    return generationStep;
}




/**
 * Lecture et display
 */

struct GenerationStepReadThreadData {
    struct GenerationStep** steps;
    struct ListNode* nodeList;
    struct Game* game;
};

void GenerationSteps_Read(struct GenerationStep** steps, struct LabyrintheNode* rootNode)
{
    if(currentGame->is_generation_steps_displayer_running)
    {
        printf("Impossible d'afficher la génération car un affichage de génération est déjà en cours.\n");
        return;
    }

    if(steps == NULL) {
        printf("Erreur: steps est NULL\n");
        return;
    }

    if(rootNode == NULL) {
        printf("Erreur: rootNode est NULL\n");
        return;
    }

    // Vérifier que le labyrinthe a des GameObjects associés
    if(rootNode->associatedGameObject == NULL) {
        printf("Erreur: Le labyrinthe n'a pas été affiché. Les GameObjects n'existent pas encore.\n");
        printf("Le labyrinthe doit être affiché avant de pouvoir rejouer la génération.\n");
        return;
    }

    // Réinitialiser l'affichage (cacher toutes les cellules)
    GameFacade_ResetLabyrintheDisplay(rootNode);

    struct ListNode* nodes = GameFacade_Labyrinthe_Nodes_To_Tab(rootNode);

    struct GenerationStepReadThreadData* generationStepReadThreadData = malloc(sizeof(struct GenerationStepReadThreadData));
    generationStepReadThreadData->nodeList = nodes;
    generationStepReadThreadData->game = currentGame;
    generationStepReadThreadData->steps = steps;

    currentGame->is_generation_steps_displayer_running = true;
    SDL_Thread *displayThread = SDL_CreateThread(displayGenerationSteps, "DisplayGeneration", (void *)generationStepReadThreadData);
}

int displayGenerationSteps(void *data) {
    struct GenerationStepReadThreadData* generationStepReadThreadData = (struct GenerationStepReadThreadData*) data;
    int stepIterator = 0;
    int currentGenIteration = 0;

    struct GenerationStep** pGenerationStep = generationStepReadThreadData->steps;

    if(pGenerationStep == NULL) {
        printf("Erreur: steps est NULL\n");
        generationStepReadThreadData->game->is_generation_steps_displayer_running = false;
        return 1;
    }

    struct GenerationStep* currentGenerationStep = pGenerationStep[0];

    if(currentGenerationStep == NULL) {
        printf("Erreur: première step est NULL\n");
        generationStepReadThreadData->game->is_generation_steps_displayer_running = false;
        return 1;
    }

    while(currentGenerationStep != NULL && currentGenerationStep->stepType != End)
    {
        // Si c'est une itération supérieure, on attend avant de la jouer
        if(currentGenerationStep->iteration > currentGenIteration)
        {
            SDL_Delay(5);
            currentGenIteration++;

            // Jouer un son toutes les 10 itérations pour garder l'animation rapide sans QUE SA ENCULE LES OREILLES
            if(currentGenIteration % INTERVAL_OF_ITERATION_FOR_EVERY_SFX == 0) {
                SFX_RandomGenSound();
            }
        }

        // On continue de lire toutes les steps d'itération currentGenIteration
        switch(currentGenerationStep->stepType)
        {
            case HighlightExistingNode:
                struct HighlightExistingStepType* step = (struct HighlightExistingStepType*) currentGenerationStep->step;
                if(step == NULL) {
                    printf("Erreur: step data est NULL pour HighlightExistingNode\n");
                    break;
                }
                int x = step->x;
                int y = step->y;

                struct LabyrintheNode* current = LabyrintheNode_Get_labyrinthe_node_at_coords(generationStepReadThreadData->nodeList, x, y);

                if(current != NULL && current->associatedGameObject != NULL) {
                    current->associatedGameObject->color = step->color;
                }

            break;

            case SetNodeVisibility:
                struct SetNodeVisibilityStepType* step2 = (struct SetNodeVisibilityStepType*) currentGenerationStep->step;
                if(step2 == NULL) {
                    printf("Erreur: step data est NULL pour SetNodeVisibility\n");
                    break;
                }
                int x2 = step2->x;
                int y2 = step2->y;

                struct LabyrintheNode* current2 = LabyrintheNode_Get_labyrinthe_node_at_coords(generationStepReadThreadData->nodeList, x2, y2);

                if(current2 != NULL && current2->associatedGameObject != NULL) {
                    current2->associatedGameObject->isVisible = step2->visible;
                }
                break;
            case End:
            default:
                break;
        }


        stepIterator++;
        currentGenerationStep = pGenerationStep[stepIterator];
    }

    // Restaurer les couleurs par défaut à la fin
    for(int i = 0; i < generationStepReadThreadData->nodeList->size; i++) {
        struct LabyrintheNode* node = generationStepReadThreadData->nodeList->nodeTab[i];
        if(node != NULL && node->associatedGameObject != NULL) {
            // Restaurer la couleur par défaut selon le type
            node->associatedGameObject->color = node->associatedGameObject->defaultColor;
        }
    }

    generationStepReadThreadData->game->is_generation_steps_displayer_running = false;
    return 0;
}

