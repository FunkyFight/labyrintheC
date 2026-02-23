//
// Created by Funky on 20/02/2026.
//

#ifndef LABYRINTHE_SFX_H
#define LABYRINTHE_SFX_H


#include <SDL3/SDL_audio.h>

struct Sound {
    const char *name;
};

void SFX_PlaySound(const char* filename);
void SFX_RandomGenSound();

#endif //LABYRINTHE_SFX_H
