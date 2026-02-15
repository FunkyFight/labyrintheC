//
// Created by Funky on 16/12/2025.
//

#include <glib.h>
#include <stdio.h>
#include "gameobject_manager.h"

void GameObjectManager_AddGameObject(struct Game *g, void *gameObjectContainer) {

    if(g->gameobjects == NULL) return;

    g_array_append_vals(g->gameobjects, &gameObjectContainer, 1);


}

int GameObjectManager_ContainsGameObject(struct Game *g, void *gameObjectContainer) {
    GArray  *array = g->gameobjects;

    for(guint i = 0; i < array->len; i++) {
        void *currentGameObjectContainer = g_array_index(array, void*, i);

        if(gameObjectContainer == currentGameObjectContainer) return i;
    }

    return -1;
}

void GameObjectManager_RemoveGameObject(struct Game *g, void *gameObjectContainer) {

    int contains = GameObjectManager_ContainsGameObject(g, gameObjectContainer);
    if(contains == -1) return;

    g_array_remove_index(g->gameobjects, contains);
}

void GameObjectManager_RemoveAllGameObjectsOfType(struct Game *g, char* type)
{
    for(guint i = 0; i < g->gameobjects->len; i++)
    {
        void* concrete = g_array_index(g->gameobjects, void*, i);
        struct GameObject* gameObject = GameObjectManager_GetGameObject(concrete);

        if(gameObject->type == type)
        {
            g_array_remove_index(g->gameobjects, i);
            gameObject->destroy_game_object(concrete);
        }
    }
}


struct GameObject* GameObjectManager_GetGameObject(void *gameObjectContainer) {
    if (gameObjectContainer == NULL) {
        return NULL;
    }

    return *(struct GameObject **)gameObjectContainer;
}
