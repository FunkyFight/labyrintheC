//
// Created by Funky on 19/02/2026.
//

#include <stdio.h>
#include "GenerationSteps.h"
#include "../../types.h"
#include "../../facade/GameFacade.h"
#include "../nodes/labyrinthe_node.h"

struct GenerationStep *GenerationSteps_Create()
{
    struct GenerationStep* strct = malloc(sizeof(struct GenerationStep**) * 9999);
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

    generationStep->step = &step;
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

    generationStep->step = &step;
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
        printf("Impossible d'afficher la génération car un affichage de génération est déjà en cours.");
        return;
    }

    struct ListNode* nodes = GameFacade_Labyrinthe_Nodes_To_Tab(rootNode);

    struct GenerationStepReadThreadData* generationStepReadThreadData = malloc(sizeof(struct GenerationStepReadThreadData*));
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
    struct GenerationStep* currentGenerationStep = pGenerationStep[0];


    while(currentGenerationStep->stepType != End)
    {
        // Si c'est une itération supérieure, on attend avant de la jouer
        if(currentGenerationStep->iteration > currentGenIteration)
        {
            SDL_Delay(250);
            currentGenIteration++;
        }

        // On continue de lire toutes les steps d'itération currentGenIteration
        switch(currentGenerationStep->stepType)
        {
            case HighlightExistingNode:
                struct HighlightExistingStepType* step = (struct HighlightExistingStepType*) currentGenerationStep->step;
                int x = step->x;
                int y = step->y;

                struct LabyrintheNode* current = LabyrintheNode_Get_labyrinthe_node_at_coords(generationStepReadThreadData->nodeList, x, y);

                current->associatedGameObject->color = step->color;

            break;

            case SetNodeVisibility:
                struct SetNodeVisibilityStepType* step2 = (struct SetNodeVisibilityStepType*) currentGenerationStep->step;
                int x2 = step->x;
                int y2 = step->y;

                struct LabyrintheNode* current2 = LabyrintheNode_Get_labyrinthe_node_at_coords(generationStepReadThreadData->nodeList, x, y);

                current->associatedGameObject->isVisible = step2->visible;
                break;
            case End:
            default:
                break;
        }

        stepIterator++;
        currentGenerationStep = pGenerationStep[stepIterator];
    }
}

