//
// Created by Funky on 16/12/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include "gameobject_labyrinthe_cell.h"

struct GameObjectLabyrintheCell* GameObject_LabyrintheCell_Create(struct Game *game, float x, float y, float c, SDL_Color color, char valueChar, struct LabyrintheNode* associatedLabyrintheNode) {
    struct GameObjectLabyrintheCell *cell = malloc(sizeof(struct GameObjectLabyrintheCell));

    struct GameObject *gameObject = malloc(sizeof(struct GameObject));

    if(gameObject == NULL || cell == NULL) {
        free(gameObject);
        free(cell);
        return NULL;
    }

    gameObject->game = game;
    gameObject->x = x;
    gameObject->y = y;
    UuidCreate(&gameObject->guid);
    gameObject->draw_game_object = (void (*)(void *)) GameObject_LabyrintheCell_Draw;
    gameObject->destroy_game_object = (void (*)(void *)) GameObject_LabyrintheCell_Destroy;
    gameObject->type = "CELL";


    cell->gameObject = gameObject;
    cell->defaultColor = color;
    cell->color = color;
    cell->c = c;
    cell->valueChar = valueChar;
    cell->isVisible = true;
    cell->associatedLabyrintheNode = associatedLabyrintheNode;
    associatedLabyrintheNode->associatedGameObject = cell;

    return cell;
}

void GameObject_LabyrintheCell_Destroy(void* gameObjectContainer) {
    struct GameObjectLabyrintheCell *cell = (struct GameObjectLabyrintheCell *) gameObjectContainer;

    free(cell->gameObject);
    free(cell);
}

void GameObject_LabyrintheCell_Draw(void *gameObjectContainer) {

    struct GameObjectLabyrintheCell *gameObjectLabyrintheCell = (struct GameObjectLabyrintheCell *)gameObjectContainer;

    SDL_FRect rect;
    rect.x = gameObjectLabyrintheCell->gameObject->x;
    rect.y = gameObjectLabyrintheCell->gameObject->y;
    rect.w = gameObjectLabyrintheCell->c;
    rect.h = gameObjectLabyrintheCell->c;

    SDL_Renderer *renderer = gameObjectLabyrintheCell->gameObject->game->renderer;
    SDL_SetRenderDrawColor(renderer, gameObjectLabyrintheCell->color.r, gameObjectLabyrintheCell->color.g, gameObjectLabyrintheCell->color.b, (gameObjectLabyrintheCell->isVisible ? gameObjectLabyrintheCell->color.a : 0));

    // @TODO: ECRIRE LE CARACTERE DE VALEUR

    SDL_RenderFillRect(renderer, &rect);
}
