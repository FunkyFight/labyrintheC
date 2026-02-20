//
// Created by Funky on 19/02/2026.
//

#include "GenerationSteps.h"
#include "../../types.h"

struct GenerationStep *GenerationSteps_Create()
{
    struct GenerationStep* strct = malloc(sizeof(struct GenerationStep*));
    strct->stepsLength = 0;
    return strct;
}

void GenerationSteps_Free(struct GenerationStep* strct)
{
    free(strct);
}

void GenerationSteps_AddStepToStruct(struct GenerationStep* strct, void* step)
{
    enum StepType type = strct->stepType;

    switch (type) {
        case SetNodeVisibility:
            strct->step[strct->stepsLength] = (struct SetNodeVisibilityStepType*) step;
            break;
        case HighlightExistingNode:
            strct->step[strct->stepsLength] = (struct HighlightExistingStepType*) step;
            break;
        case End:
            strct->step[strct->stepsLength] = (struct End*) step;
            break;
    }

    strct->stepsLength++;
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

