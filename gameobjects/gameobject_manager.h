//
// Created by Funky on 16/12/2025.
//

#ifndef LABYRINTHE_GAMEOBJECT_MANAGER_H
#define LABYRINTHE_GAMEOBJECT_MANAGER_H
#include "../types.h"

void GameObjectManager_AddGameObject(struct Game *g, void *gameObjectContainer);
void GameObjectManager_RemoveGameObject(struct Game *g, void *gameObjectContainer);
int GameObjectManager_ContainsGameObject(struct Game *g, void *gameObjectContainer);
struct GameObject* GameObjectManager_GetGameObject(void *gameObjectContainer);
void GameObjectManager_RemoveAllGameObjectsOfType(struct Game *g, char* type);


#endif //LABYRINTHE_GAMEOBJECT_MANAGER_H
