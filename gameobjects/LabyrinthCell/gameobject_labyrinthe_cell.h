//
// Created by Funky on 16/12/2025.
//

#ifndef LABYRINTHE_GAMEOBJECT_LABYRINTHE_CELL_H
#define LABYRINTHE_GAMEOBJECT_LABYRINTHE_CELL_H

#include "../../types.h"

struct GameObjectLabyrintheCell {
    struct GameObject *gameObject;
    float c; // Taille d'un côté
    SDL_Color color;
    char valueChar;
};

struct GameObjectLabyrintheCell* GameObject_LabyrintheCell_Create(struct Game *game, float x, float y, float c, SDL_Color color, char valueChar);
void GameObject_LabyrintheCell_Destroy(struct GameObjectLabyrintheCell *cell);
void GameObject_LabyrintheCell_Draw(void *gameObjectContainer);



#endif //LABYRINTHE_GAMEOBJECT_LABYRINTHE_CELL_H
