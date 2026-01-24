#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gameobject_labyrinthe_cell.h"

//pas de caractere précis pour CELL ou WALL !? Ca marche avec la couleur !?

// Charge le JSON -> grille
int Labyrinthe_LoadJSON(struct Game *game, struct GameObjectLabyrintheCell ***grid, int width, int height, const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (!file) {
        perror("Erreur ouverture fichier");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int x, y;
        float c;
        char valueChar;
        int r, g, b, a;

        //ligne JSON : { "x": 0, "y": 1, "c": 32.00, "valueChar": "A", "color": { "r": 255, "g": 0, "b": 0, "a": 255 } }

        int matched = sscanf(
            line,
            " { \"x\": %d, \"y\": %d, \"c\": %f, \"valueChar\": \"%c\", \"color\": { \"r\": %d, \"g\": %d, \"b\": %d, \"a\": %d } }",
            &x, &y, &c, &valueChar, &r, &g, &b, &a
        );

        if (matched == 8) {
            SDL_Color color = { r, g, b, a };

            struct GameObjectLabyrintheCell *cell = GameObject_LabyrintheCell_Create(game, (float)x * c, (float)y * c, c, color, valueChar);
            if (!cell) {
                fprintf(stderr, "Erreur création cellule x=%d y=%d\n", x, y);
                continue;
            }

            grid[x][y] = cell;
        }
    }

    fclose(file);

    // Reconstruction des voisins
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            struct GameObjectLabyrintheCell *cell = grid[x][y];
            if (!cell) continue;

            if (y > 0)            cell->north = grid[x][y - 1];
            if (y < height - 1)   cell->south = grid[x][y + 1];
            if (x > 0)            cell->west  = grid[x - 1][y];
            if (x < width - 1)    cell->east  = grid[x + 1][y];
        }
    }

    return 0;
}
