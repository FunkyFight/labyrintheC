//
// Created by rapha on 17/02/2026.
//

#include "random_solver.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../../types.h"







struct LabyrintheNode* Rondom_Solver(struct LabyrintheNode* node)
{
    if (node->type == START) {
        int aleatoire = rand() % (4);
        switch (aleatoire)
        {
            case 0:
                if (node->north->type == CELL)
                {
                    node->color = (SDL_Color){0, 255, 0, 255};
                    node->north = Rondom_Solver(node->north);
                }
                break;
            case 1:
                if (node->west->type == CELL)
                {
                    node->color = (SDL_Color){0, 255, 0, 255};
                    node->west = Rondom_Solver(node->west);
                }
                break;
            case 2:
                if (node->east->type == CELL)
                {
                    node->color = (SDL_Color){0, 255, 0, 255};
                    node->east = Rondom_Solver(node->east);
                }
                break;
            case 3:
                if (node->south->type == CELL)
                {
                    node->color = (SDL_Color){0, 255, 0, 255};
                    node->south = Rondom_Solver(node->south);
                }
                break;
            default:
                fprintf(stderr, "Erreur: type de node inattendu (%d)\n", node->type);
                exit(EXIT_FAILURE);
        }



    }
    else if (node->type == END)
    {
        printf("GG");

    }
    else
    {
        fprintf(stderr, "Erreur: type de node inattendu (%d)\n", node->type);
        exit(EXIT_FAILURE);
    }
    return node;
}