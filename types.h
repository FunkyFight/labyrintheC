//
// Created by Funky on 15/12/2025.
//

#ifndef LABYRINTHE_TYPES_H
#define LABYRINTHE_TYPES_H

#include <SDL3/SDL.h>
#include <rpc.h>
#include <glib.h>
#include "gameobjects/LabyrinthCell/gameobject_labyrinthe_cell.h"


/**
 * Structures visualisation step by step de la génération
 */

enum StepType
{
    HighlightExistingNode,
    SetNodeVisibility,
    End /* ULTRA IMPORTANT, A METTRE DANS LE TABLEAU A LA FIN, EQUIVALENT DU \0 POUR UNE CHAINE */
};

struct HighlightExistingStepType {
    int x, y;
    SDL_Color color;
};

struct SetNodeVisibilityStepType {
    int x, y;
    bool visible;
};

struct GenerationStep {
    void* step;
    enum StepType stepType;
    int iteration; // C'est l'itération numéro combien de la génération ?
};

/**
 * Structures Labyrinthe
 */

// Types de noeuds dans le labyrinthe
enum LabyrintheNodeType {
    CELL, // Cellule normale où on peut marcher
    WALL, // Mur infranchissable
    START, // Départ
    END // Arrivée
};

// Structure d'un noeud de labyrinthe. Contient les voisins, le type, le coût en voyage...
struct LabyrintheNode {
    struct LabyrintheNode* north;
    struct LabyrintheNode* south;
    struct LabyrintheNode* east;
    struct LabyrintheNode* west;

    int x, y;

    enum LabyrintheNodeType type;
    int travel_cost;

    struct GameObjectLabyrintheCell* associatedGameObject;

    bool visited;
};

// Labyrinthe
struct Labyrinthe {
    char *filePath;
    char *name;

    int width, height; // Taille du labyrinthe

    struct LabyrintheNode* firstNode; // Premier noeud du labyrinthe
};

struct LabyrintheCases {
    char *filePath;
    char *name;

    int width, height; // Taille du labyrinthe

    struct LabyrintheNode* nodes[];
};

/**
 * TYPES POUR LA FENÊTRE
 */
struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event *event;
    bool is_running;
    GArray *gameobjects;
    struct Labyrinthe *labyrinthe;

    bool is_generation_steps_displayer_running;
    struct GenerationStep** generationSteps; // Étapes de générations pouvant être display quand on presse SPACE


    // Algos
    struct ListNode* (*gen_algorithm)(int height,int width, struct LabyrintheNode* starter, struct GenerationStep** steps);
};

typedef struct AlgoGenFunctionContainer {
    struct ListNode* (*gen_algorithm)(int height,int width, struct LabyrintheNode* starter, struct GenerationStep** steps);
} AlgoGenFunctionContainer;

struct GameObject {
    GUID guid;
    int zindex;
    struct Game *game;
    float x, y;
    void (*draw_game_object)(void *g);
    void (*destroy_game_object)(void *g);
    void (*event_on_click)(void *g);
    char* type;
};

struct BaseGameObject {
    struct GameObject *gameObject;
};

#endif //LABYRINTHE_TYPES_H
