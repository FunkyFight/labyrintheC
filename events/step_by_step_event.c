//
// Created by Funky on 19/02/2026.
//

#include "step_by_step_event.h"
#include "../facade/GameFacade.h"
#include <stdio.h>

bool apply_next_step(struct Game *g) {
    if (g == NULL || g->stepsToShow == NULL) {
        printf("apply_next_step: g=%p, stepsToShow=%p\n", (void*)g, g ? (void*)g->stepsToShow : NULL);
        return false;
    }

    printf("apply_next_step called for iteration %d\n", g->currentIteration);

    // Parcourir toutes les étapes avec l'itération actuelle
    int stepsApplied = 0;
    for (int i = 0; g->stepsToShow[i] != NULL; i++) {
        struct GenerationStep *step = g->stepsToShow[i];

        if (step->iteration != g->currentIteration) continue;

        stepsApplied++;

        // Appliquer l'étape selon son type
        switch (step->stepType) {
            case SetNodeVisibility: {
                struct SetNodeVisibilityStepType *visStep = (struct SetNodeVisibilityStepType *)step->step;

                struct LabyrintheNode *node = GameFacade_FindNodeByCoords(g->labyrinthe, visStep->x, visStep->y);
                if (node != NULL) {
                    node->visible = visStep->visible;
                    // Mettre à jour l'alpha du cell
                    if (node->cell != NULL) {
                        if (visStep->visible) {
                            node->cell->color.a = 255;
                        } else {
                            node->cell->color.a = 0;
                        }
                    } else {
                        printf("Warning: node at (%d,%d) has no cell\n", visStep->x, visStep->y);
                    }
                }
                break;
            }

            case HighlightExistingNode: {
                struct HighlightExistingStepType *hlStep = (struct HighlightExistingStepType *)step->step;

                struct LabyrintheNode *node = GameFacade_FindNodeByCoords(g->labyrinthe, hlStep->x, hlStep->y);
                if (node != NULL && node->cell != NULL) {
                    // Changer la couleur du cell
                    node->cell->color = hlStep->color;
                    // Respecter la visibilité
                    if (node->visible) {
                        node->cell->color.a = 255;
                    } else {
                        node->cell->color.a = 0;
                    }
                }
                break;
            }
        }
    }

    if (stepsApplied > 0) {
        printf("Iteration %d: applied %d steps\n", g->currentIteration, stepsApplied);
        g->currentIteration++;
        return true;
    } else {
        printf("Animation finished\n");
        g->isPlayingSteps = false;
        return false;
    }
}

void event_on_space_key(struct Game *g) {
    if (g == NULL) return;

    printf("SPACE pressed! isPlayingSteps was: %d\n", g->isPlayingSteps);

    // Toggle l'auto-play
    g->isPlayingSteps = !g->isPlayingSteps;

    if (g->isPlayingSteps) {
        printf("Starting animation at iteration %d\n", g->currentIteration);
        g->lastStepTime = SDL_GetTicks();
    } else {
        printf("Pausing animation\n");
    }
}
