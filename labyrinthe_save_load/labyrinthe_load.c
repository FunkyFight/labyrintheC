#include <stdio.h>
#include <stdlib.h>
#include "../types.h"
#include "../business/nodes/labyrinthe_node.h"

struct Labyrinthe* Labyrinthe_LoadJSON(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Erreur ouverture fichier");
        return NULL;
    }

    struct Labyrinthe *labyrinthe = malloc(sizeof(struct Labyrinthe));
    if (!labyrinthe) { fclose(file); return NULL; }

    labyrinthe->filePath = NULL;
    labyrinthe->name = NULL;
    labyrinthe->firstNode = NULL;
    labyrinthe->width = 0;
    labyrinthe->height = 0;

    // Lire width et height depuis le JSON
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, " \"width\": %d", &labyrinthe->width) == 1) continue;
        if (sscanf(line, " \"height\": %d", &labyrinthe->height) == 1 ) continue;
        if (strstr(line, "\"nodes\"")) break;
    }

    int width = labyrinthe->width;
    int height = labyrinthe->height;

    if (width<=0 || height<=0) {
        fprintf(stderr, "Dimensions invalides: %dx%d\n", width, height);
        free(labyrinthe); fclose(file); return NULL;
    }

    struct LabyrintheNode ***grid = malloc(width * sizeof(struct LabyrintheNode**));
    if (!grid) { free(labyrinthe); fclose(file); return NULL; }
    for (int i = 0; i < width; i++) {
        grid[i] = calloc(height, sizeof(struct LabyrintheNode*));
    }

    // Lire les nodes
    int x, y; char typeStr[16];

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line,
            " { \"x\": %d , \"y\": %d , \"type\": \"%15[^\"]\"",
            &x,&y, typeStr) == 3) {

            if (x < 0 || x >= width || y < 0 || y >= height) {
                fprintf(stderr, "Node hors bornes ignoré: x=%d y=%d\n", x, y);
                continue;
            }

            enum LabyrintheNodeType type = CELL;
            if (strcmp(typeStr, "WALL") == 0) type = WALL;
            else if (strcmp(typeStr, "START") == 0) type = START;
            else if (strcmp(typeStr, "END") == 0) type = END;

            struct LabyrintheNode *node = LabyrintheNode_CreateCoords(x,y, 1);
            if (!node) { fprintf(stderr, "Erreur de l'allocation du Node: x=%d y=%d\n", x, y); continue; }
            node->type = type;
            grid[x][y] = node;

            if (type == START) { labyrinthe->firstNode = node; }
        }
    }

    // Relier voisins
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            struct LabyrintheNode *node = grid[x][y];
            if (!node) continue;

            // Vérification que le node est bien à la bonne place
            if (node->x != x || node->y != y) {
                fprintf(stderr, "INCOHÉRENCE: grid[%d][%d] contient node->x=%d node->y=%d\n",
                        x, y, node->x, node->y);
            }

            node->north = (y > 0)          ? grid[x][y-1] : NULL;
            node->south = (y < height - 1) ? grid[x][y+1] : NULL;
            node->west  = (x > 0)          ? grid[x-1][y] : NULL;
            node->east  = (x < width - 1)  ? grid[x+1][y] : NULL;
        }
    }

    if (!labyrinthe->firstNode) {
        fprintf(stderr, "Aucun START trouvé, utilisation de grid[0][0]\n");
        if (grid[0][0]) {
            labyrinthe->firstNode = grid[0][0];
            } else {
                for (int i = 0; i < width; i++) free(grid[i]);
                    free(grid); free(labyrinthe); fclose(file); return NULL;
                }
    }

    // Libération de la mémoire
    for (int x = 0; x < labyrinthe->width; x++) {
        free(grid[x]);
    }
    free(grid);

    fclose(file);
    return labyrinthe;
}