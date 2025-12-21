//
// Created by Funky on 15/12/2025.
//

#include <glib.h>
#include <stdio.h>
#include "draw_game.h"
#include "../gameobjects/LabyrinthCell/gameobject_labyrinthe_cell.h"
#include "../gameobjects/gameobject_manager.h"

static GArray *renderable_gameobjects;

void draw_init(struct Game *g) {
    struct SDL_Color cellColor = {39, 245, 53, 255};
    struct GameObjectLabyrintheCell *cell = GameObject_LabyrintheCell_Create(g, 400, 300, 50, cellColor, '\0');

    GameObjectManager_AddGameObject(g, cell);

}

void draw_destroy(struct Game *g) {

}

void draw_game(struct Game *g) {


    SDL_SetRenderDrawColor(g->renderer, 245, 39, 183, 255);
    SDL_RenderClear(g->renderer);

    GArray *gameObjects = g->gameobjects;

    for(guint i = 0; i < gameObjects->len; i++) {
        void* concrete = g_array_index(gameObjects, void*, i);
        struct GameObject *gameObject = GameObjectManager_GetGameObject(concrete);

        gameObject->draw_game_object(concrete);
    }

    SDL_RenderPresent(g->renderer);
}
