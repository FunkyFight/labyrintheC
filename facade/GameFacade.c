//
// Created by Funky on 30/01/2026.
//

#include "GameFacade.h"
#include <stdio.h>
#include "../business/nodes/labyrinthe_node.h"
#include "../generation/RDFS/rdfsGeneration.h"

static void GameFacade_ShowNode(struct Game* game, struct LabyrintheNode* node, int cellSx, int cellSy);
static void GameFacade_ResetVisited(struct LabyrintheNode* node);
static bool Labyrinthe_ValidateGrid(struct Labyrinthe* labyrinthe);
static void Labyrinthe_FreeGrid(struct Labyrinthe* labyrinthe);

void printRoad(ListNode* road) {
    if (!road) return;

    for (int i = 0; i < road->size; i++) {
        LabyrintheNode* n = road->nodeTab[i];
        printf("(%d,%d:%d) ", n->x, n->y, n->travel_cost);
    }
    printf("\n");
}
void printRoad2D(ListNode* road, int height, int width) {
    if (!road) return;

    int grid[height][width];

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x] = -1;
        }
    }

    for (int i = 0; i < road->size; i++) {
        LabyrintheNode* n = road->nodeTab[i];
        if (n->x >= 0 && n->x < width && n->y >= 0 && n->y < height) {
            grid[n->y][n->x] = n->travel_cost;
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x] == -1)
                printf(" . ");
            else
                printf("%2d ", grid[y][x]);
        }
        printf("\n");
    }
}


void DebugAndTest()
{
    /**
     * ULTRA IMPORTANT A LIRE
     * Utilisez cette fonction pour faire des appels de fonctions de test. Tout ce que vous exécuterez ici sera exécutée par main().
     * Donc pas besoin de toucher au main !
     */
    srand(time(NULL));

    int height = 10;
    int width = 10;

    //Les 2 fonctionnent
    ListNode* road = fullFillLabyrintheGeneration(height, width,0,NULL);
    //ListNode* road = fullFillLabyrintheGeneration(height, width,0,LabyrintheNode_CreateCoords(4,7,randomTravelCost()));

    printf("Affichage lineaire :\n");
    printRoad(road);

    printf("\nAffichage 2D :\n");
    printRoad2D(road, height, width);

    freeListNode(road);

    struct Labyrinthe labyrinthe = {0};
    bool ok = Labyrinthe_ValidateGrid(&labyrinthe);
    GameFacade_ShowInstantlyLabyrinthe(&labyrinthe);
}

static bool Labyrinthe_ValidateGrid(struct Labyrinthe* labyrinthe) {
    if(labyrinthe == NULL) return false;
    if(labyrinthe->firstNode == NULL) return false;
    if(labyrinthe->width <= 0 || labyrinthe->height <= 0) return false;

    int w = labyrinthe->width;
    int h = labyrinthe->height;
    int max = w * h;
    if(max <= 0) return false;

    // Tableau d'indexation par coordonnées:
    // nodes[y*w + x] == pointeur unique du noeud (x,y) rencontré pendant le parcours.
    // Sert à:
    // - marquer "déjà visité"
    // - détecter collisions (2 pointeurs différents pour la même coordonnée)
    // - garantir qu'on a bien w*h noeuds.
    struct LabyrintheNode** nodes = calloc(max, sizeof(*nodes));
    if(nodes == NULL) return false;

    // Pile explicite pour un DFS itératif (évite la récursion).
    int stackCap = max * 4;
    if(stackCap < 16) stackCap = 16;
    struct LabyrintheNode** stack = malloc((size_t)stackCap * sizeof(*stack));
    if(stack == NULL) {
        free(nodes);
        return false;
    }

    int top = 0;
    stack[top++] = labyrinthe->firstNode;

    bool ok = true;
    int count = 0;

    while(top > 0) {
        struct LabyrintheNode* node = stack[--top];
        if(node == NULL) continue;

        if(node->x < 0 || node->x >= w || node->y < 0 || node->y >= h) {
            ok = false;
            continue;
        }

        int idx = node->y * w + node->x;
        if(nodes[idx] != NULL) {
            if(nodes[idx] != node) ok = false;
            continue;
        }

        // Première fois qu'on rencontre ce (x,y).
        nodes[idx] = node;
        count++;

        // Si on manque de place, on agrandit la pile.
        if(top + 4 >= stackCap) {
            int newCap = stackCap * 2;
            struct LabyrintheNode** newStack = realloc(stack, (size_t)newCap * sizeof(*newStack));
            if(newStack == NULL) {
                ok = false;
                break;
            }
            stack = newStack;
            stackCap = newCap;
        }

        // On empile les voisins (la déduplication se fait via nodes[idx]).
        stack[top++] = node->north;
        stack[top++] = node->south;
        stack[top++] = node->east;
        stack[top++] = node->west;
    }

    // On doit pouvoir atteindre exactement w*h noeuds.
    if(count != max) ok = false;

    // Si le comptage est bon, on vérifie la cohérence des voisins:
    // - aux bords, pas de voisin vers l'extérieur
    // - ailleurs, voisin non-NULL sur la case adjacente attendue
    // - liens bidirectionnels: east->west == node, etc.
    if(ok) {
        for(int y = 0; y < h; y++) {
            for(int x = 0; x < w; x++) {
                struct LabyrintheNode* node = nodes[y * w + x];
                if(node == NULL) {
                    ok = false;
                    continue;
                }

                if(x > 0) {
                    if(node->west == NULL || node->west->x != x - 1 || node->west->y != y) ok = false;
                } else {
                    if(node->west != NULL) ok = false;
                }

                if(x < w - 1) {
                    if(node->east == NULL || node->east->x != x + 1 || node->east->y != y) ok = false;
                } else {
                    if(node->east != NULL) ok = false;
                }

                if(y > 0) {
                    if(node->north == NULL || node->north->x != x || node->north->y != y - 1) ok = false;
                } else {
                    if(node->north != NULL) ok = false;
                }

                if(y < h - 1) {
                    if(node->south == NULL || node->south->x != x || node->south->y != y + 1) ok = false;
                } else {
                    if(node->south != NULL) ok = false;
                }

                if(node->east != NULL && node->east->west != node) ok = false;
                if(node->west != NULL && node->west->east != node) ok = false;
                if(node->north != NULL && node->north->south != node) ok = false;
                if(node->south != NULL && node->south->north != node) ok = false;
            }
        }
    }

    free(stack);
    free(nodes);
    return ok;
}

static void Labyrinthe_FreeGrid(struct Labyrinthe* labyrinthe) {
    if(labyrinthe == NULL) return;
    if(labyrinthe->firstNode == NULL) return;
    if(labyrinthe->width <= 0 || labyrinthe->height <= 0) return;

    int w = labyrinthe->width;
    int h = labyrinthe->height;
    int max = w * h;
    if(max <= 0) return;

    // Même mécanisme d'indexation que ValidateGrid:
    // nodes[y*w + x] permet de s'assurer qu'on ne free chaque noeud qu'une seule fois.
    struct LabyrintheNode** nodes = calloc(max, sizeof(*nodes));
    if(nodes == NULL) return;

    // Pile explicite pour parcourir la structure sans récursion.
    int stackCap = max * 4;
    if(stackCap < 16) stackCap = 16;
    struct LabyrintheNode** stack = malloc((size_t)stackCap * sizeof(*stack));
    if(stack == NULL) {
        free(nodes);
        return;
    }

    int top = 0;
    stack[top++] = labyrinthe->firstNode;

    while(top > 0) {
        struct LabyrintheNode* node = stack[--top];
        if(node == NULL) continue;
        if(node->x < 0 || node->x >= w || node->y < 0 || node->y >= h) continue;

        int idx = node->y * w + node->x;
        // Déjà indexé => déjà vu, on ne repasse pas dessus (évite double-free).
        if(nodes[idx] != NULL) continue;
        nodes[idx] = node;

        // Agrandir la pile si nécessaire.
        if(top + 4 >= stackCap) {
            int newCap = stackCap * 2;
            struct LabyrintheNode** newStack = realloc(stack, (size_t)newCap * sizeof(*newStack));
            if(newStack == NULL) break;
            stack = newStack;
            stackCap = newCap;
        }

        // Empiler les voisins pour indexation.
        stack[top++] = node->north;
        stack[top++] = node->south;
        stack[top++] = node->east;
        stack[top++] = node->west;
    }

    // Free en une passe: chaque case non-NULL correspond à un pointeur unique de noeud.
    for(int i = 0; i < max; i++) {
        if(nodes[i] != NULL) free(nodes[i]);
    }

    free(stack);
    free(nodes);

    // Le Labyrinthe ne référence plus une grille valide.
    labyrinthe->firstNode = NULL;
    labyrinthe->width = 0;
    labyrinthe->height = 0;
}


void GameFacade_ShowInstantlyLabyrinthe(struct Labyrinthe* labyrinthe)
{
    if(currentGame == NULL) return;
    if(labyrinthe == NULL) return;
    if(labyrinthe->firstNode == NULL) return;
    if(labyrinthe->width <= 0 || labyrinthe->height <= 0) return;

    struct Game* game = (struct Game*) currentGame;
    game->labyrinthe = labyrinthe;

    GameObjectManager_RemoveAllGameObjectsOfType(game, "CELL");

    int w, h;
    SDL_GetWindowSize(game->window, &w, &h);

    // Calcul de la taille d'une cellule pour la taille du labyrinthe
    int cellSx = w / labyrinthe->width;
    int cellSy = h / labyrinthe->height;
    if(cellSx <= 0) cellSx = 1;
    if(cellSy <= 0) cellSy = 1;

    GameFacade_ResetVisited(labyrinthe->firstNode);

    // Parcours
    GameFacade_ShowNode(game, labyrinthe->firstNode, cellSx, cellSy);
}

static void GameFacade_ResetVisited(struct LabyrintheNode* node) {
    if(node == NULL) return;
    if(!node->visited) return;

    node->visited = false;

    if(node->north != NULL) GameFacade_ResetVisited(node->north);
    if(node->south != NULL) GameFacade_ResetVisited(node->south);
    if(node->east != NULL) GameFacade_ResetVisited(node->east);
    if(node->west != NULL) GameFacade_ResetVisited(node->west);
}

static void GameFacade_ShowNode(struct Game* game, struct LabyrintheNode* node, int cellSx, int cellSy) {
    // Vérifier si déjà visité
    if(node->visited) return;

    // Montrer le noeud actuel
    // Pour ça on vérifie si c'est un mur ou non
    bool isWall = node->type == WALL;
    node->visited = true;

    // Déterminer la couleur
    SDL_Color cellColor = {};
    switch(node->type) {
        case WALL:
            cellColor = (SDL_Color){192, 70, 87, 1};
            break;
        case START:
            cellColor = (SDL_Color){168, 255, 101, 1};
            break;
        case END:
            cellColor = (SDL_Color){245, 255, 107, 1};
            break;
        default:
            cellColor = (SDL_Color){251, 252, 250, 1};
            break;
    }

    // Coordonnées et création
    int x = node->x * cellSx;
    int y = node->y * cellSy;

    struct GameObjectLabyrintheCell* cellGameObject = GameObject_LabyrintheCell_Create(game, x, y, cellSx, cellColor, ' '); // Toujours un carré
    GameObjectManager_AddGameObject(game, cellGameObject);

    // Récursion aux noeuds enfants
    if(node->north != NULL) GameFacade_ShowNode(game, node->north, cellSx, cellSy);
    if(node->south != NULL) GameFacade_ShowNode(game, node->south, cellSx, cellSy);
    if(node->east != NULL) GameFacade_ShowNode(game, node->east, cellSx, cellSy);
    if(node->west != NULL) GameFacade_ShowNode(game, node->west, cellSx, cellSy);

}
