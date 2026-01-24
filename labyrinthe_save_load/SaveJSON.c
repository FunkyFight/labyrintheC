#include <stdio.h>
#include <stdlib.h>
#include "gameobject_labyrinthe_cell.h"

// Compliqué avec un seul pointeur pour le parcours
// Devrait y avoir une grille 2D dans l'object du labyrinthe

void Labyrinthe_SaveJSON(struct GameObjectLabyrintheCell ***grid, int width, int height, const char *filePath) { //les param sont pas bon
    FILE *file = fopen(laby->filePath, "w");
    if (!file) {
        perror("Erreur ouverture fichier");
        return;
    }

    // Informations sur le Labirynthe
    fprintf(file, "{\n");
    fprintf(file, "  \"name\": \"%s\",\n", laby->name);
    fprintf(file, "  \"width\": %d,\n", laby->width);
    fprintf(file, "  \"height\": %d,\n", laby->height);
    fprintf(file, "  \"nodes\": [\n");

    // parcours des nodes du labyrinthe
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            struct GameObjectLabyrintheCell *cell = laby->grid[x][y];
            if (!cell) continue; //ptit sécu (a voir)

            fprintf(file,
                "    { \"x\": %d, \"y\": %d, \"c\": %.2f, \"valueChar\": \"%c\", \"color\": { \"r\": %d, \"g\": %d, \"b\": %d, \"a\": %d } }",
                x, y,
                cell->c,
                cell->valueChar,
                cell->color.r, cell->color.g, cell->color.b, cell->color.a
            );

            if (!(x == laby->width - 1 && y == laby->height - 1)) { // la virgul de fin!
                fprintf(file, ",");
            }
            fprintf(file, "\n");
        }
    }

    fprintf(file, "  ]\n");
    fprintf(file, "}\n");

    fclose(file);
}

// porduit des lignes comme : { "x": 0, "y": 1, "c": 32.00, "valueChar": "A", "color": { "r": 255, "g": 0, "b": 0, "a": 255 } }
