//
// Created by Funky on 15/12/2025.
//

#include <stdio.h>
#include "keyboard_events.h"
#include "../business/generationSteps/GenerationSteps.h"

void event_on_key_down(struct Game *g) {

    SDL_Event* event = g->event;
    SDL_Keycode keycode = event->key.key;

    if (g->event->key.repeat != 0) {
        return; // On ignore la répétition automatique du système
    }

    switch(keycode)
    {
        case SDLK_SPACE:
            GenerationSteps_Read(g->generationSteps, g->labyrinthe->firstNode);
            printf("Affichage étape par étape du labyrinthe");
            break;
    }

}