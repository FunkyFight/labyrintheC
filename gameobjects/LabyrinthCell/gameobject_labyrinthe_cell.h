//
// Created by Funky on 16/12/2025.
//

#ifndef LABYRINTHE_GAMEOBJECT_LABYRINTHE_CELL_H
#define LABYRINTHE_GAMEOBJECT_LABYRINTHE_CELL_H

#include "../../types.h"

struct GameObjectLabyrintheCell {
    struct GameObject *gameObject;
    float c; // Taille d'un côté
    SDL_Color defaultColor;
    SDL_Color color;
    char valueChar;

    bool isVisible;

    struct LabyrintheNode* associatedLabyrintheNode;
};

struct GameObjectLabyrintheCell* GameObject_LabyrintheCell_Create(struct Game *game, float x, float y, float c, SDL_Color color, char valueChar, struct LabyrintheNode* associatedLabyrintheNode);
void GameObject_LabyrintheCell_Destroy(void *gameObjectContainer);
void GameObject_LabyrintheCell_Draw(void *gameObjectContainer);



#endif //LABYRINTHE_GAMEOBJECT_LABYRINTHE_CELL_H
