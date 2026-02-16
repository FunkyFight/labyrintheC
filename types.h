//
// Created by Funky on 15/12/2025.
//

#ifndef LABYRINTHE_TYPES_H
#define LABYRINTHE_TYPES_H

#include <SDL3/SDL.h>
#include <rpc.h>
#include <glib.h>




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

    bool visited;
};

// Labyrinthe
struct Labyrinthe {
    char *filePath;
    char *name;

    int width, height; // Taille du labyrinthe

    struct LabyrintheNode* firstNode; // Premier noeud du labyrinthe
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
};

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
