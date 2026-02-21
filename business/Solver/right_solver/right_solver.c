//
// Created by rapha on 16/02/2026.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../../types.h"







struct ListNode* Right_Solver(struct LabyrintheNode* node)
{

    if (node->type == START) {
        if (node->east->type == CELL)
        {
            node->color = (SDL_Color){0, 255, 0, 255};
            node->east = Right_Solver(node->east);
        }
        else if (node->north->type == CELL)
        {
            node->color = (SDL_Color){0, 255, 0, 255};
            node->north = Right_Solver(node->north);
        }
        else if (node->west->type == CELL)
        {
            node->color = (SDL_Color){0, 255, 0, 255};
            node->west = Right_Solver(node->west);
        }
        else if (node->south->type == CELL)
        {
            node->color = (SDL_Color){0, 255, 0, 255};
            node->south = Right_Solver(node->south);
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