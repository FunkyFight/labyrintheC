#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "types.h"
#include "events/event_manager.h"
#include "draw/draw_game.h"
#include "business/nodes/labyrinthe_node.h"
#include "main.h"
#include "facade/GameFacade.h"

struct Game *currentGame = NULL;

#define SDL_FLAGS (SDL_INIT_VIDEO | SDL_INIT_AUDIO)
#define WINDOWS_FLAGS (0)
#define WINDOW_TITLE "Labyrinthe - Projet Technique L3"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800



bool game_init_sdl(struct Game *g) {
    if(!SDL_Init(SDL_FLAGS)) {
        fprintf(stderr, "Erreur d'initialisation SDL 3 : %s\n", SDL_GetError());
        return false;
    }

    g->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOWS_FLAGS);
    if(!g->window) {
        fprintf(stderr, "Erreur d'initialisation fenêtre : %s\n", SDL_GetError());
        return false;
    }

    g->renderer = SDL_CreateRenderer(g->window, NULL);
    if(!g->renderer) {
        fprintf(stderr, "Erreur d'initialisation du rendu : %s\n", SDL_GetError());
        return false;
    }

    g->is_running = true;
    g->event = malloc(sizeof *g->event);

    if(g->gameobjects == NULL) {
        g->gameobjects = g_array_new(TRUE, FALSE, sizeof(gpointer));
    }

    draw_init(g);

    currentGame = g;

    // Random labyrinthe de test
    g->labyrinthe = calloc(1, sizeof(struct Labyrinthe));
    if (g->labyrinthe == NULL) {
        fprintf(stderr, "Erreur d'allocation du labyrinthe\n");
        return false;
    }

    return true;
}

void game_run(struct Game *g) {
    while(g->is_running) {

        while(SDL_PollEvent(g->event)) {
            event_manager_emit_event(g);
        }

        draw_game(g);

        SDL_Delay(16);
    }
}

void game_free(struct Game *g) {
    if(g->window) {
        SDL_DestroyWindow(g->window);
        g->window = NULL;
    }

    if(g->renderer) {
        SDL_DestroyRenderer(g->renderer);
        g->renderer = NULL;
    }

    free(g->event);

    if(g->gameobjects != NULL) {
        g_array_free(g->gameobjects, TRUE);
    }

    draw_destroy(g);

    printf("Tout a été nettoyé");

    SDL_Quit();
}

int main(int argc, char **argv) {
    bool exit_status = EXIT_FAILURE;

    struct Game game = {0};

    if(game_init_sdl(&game)) {
        exit_status = EXIT_SUCCESS;
        DebugAndTest();
        game_run(&game);
    }


    game_free(&game);



    return exit_status;
}
