//
// Created by Funky on 19/02/2026.
//

#ifndef LABYRINTHE_GENERATIONSTEPS_H
#define LABYRINTHE_GENERATIONSTEPS_H

#include <SDL3/SDL_pixels.h>
#include "../../types.h"

struct GenerationStep **GenerationSteps_Create();
void GenerationSteps_Free(struct GenerationStep** strct);
struct GenerationStep *GenerationSteps_CreateSetNodeVisibilityStep(int x, int y, bool visible, int iteration);
int displayGenerationSteps(void *data);
void GenerationSteps_Read(struct GenerationStep** steps, struct LabyrintheNode* rootNode);
struct GenerationStep *GenerationSteps_CreateHighlightExistingStepStep(int x, int y, SDL_Color color, int iteration);
#endif //LABYRINTHE_GENERATIONSTEPS_H
