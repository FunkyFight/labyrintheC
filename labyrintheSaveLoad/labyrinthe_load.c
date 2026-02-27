#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../types.h"
#include "../business/nodes/labyrinthe_node.h"

// Macro pour clean :  libère grille, labyrinthe et ferme le fichier
#define LOAD_CLEAN \
    do { \
        for (int _x = 0; _x < width; _x++) { \
            if(grid[_x]) { \
                for (int _y = 0; _y < height; _y++) { \
                    if (grid[_x][_y]) { free(grid[_x][_y]); } \
                } \
                free(grid[_x]); \
            } \
        } \
        free(grid); \
        free(labyrinthe); \
        fclose(file); \
        return NULL; \
    } while (0)


struct Labyrinthe* Labyrinthe_LoadJSON(const char *file_path) {

    // ------------------------------------------------------------------------------------
    // Ouverture du fichier
    // ------------------------------------------------------------------------------------
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Erreur ouverture fichier");
        return NULL;
    }

    // ------------------------------------------------------------------------------------
    // Allocation du fichier
    // ------------------------------------------------------------------------------------
    struct Labyrinthe *labyrinthe = malloc(sizeof(struct Labyrinthe));
    if (!labyrinthe) { fclose(file); return NULL; }

    labyrinthe->filePath = NULL;
    labyrinthe->name = NULL;
    labyrinthe->firstNode = NULL;
    labyrinthe->width = 0;
    labyrinthe->height = 0;

    // ------------------------------------------------------------------------------------
    // Lecture width et height puis les nodes depuis le JSON
    // ------------------------------------------------------------------------------------
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, " \"width\": %d",  &labyrinthe->width)  == 1) continue;
        if (sscanf(line, " \"height\": %d", &labyrinthe->height) == 1) continue;
        if (strstr(line, "\"nodes\"")) break;
    }

    int width = labyrinthe->width;
    int height = labyrinthe->height;

    if (width<=0 || height<=0) {
        fprintf(stderr, "Dimensions invalides: %dx%d\n", width, height);
        free(labyrinthe); fclose(file); return NULL;
    }

    // ------------------------------------------------------------------------------------
    // Allocation de la grille temp : grid[x][y]
    // ------------------------------------------------------------------------------------
    struct LabyrintheNode ***grid = malloc(width * sizeof(struct LabyrintheNode**));
    if (!grid) { free(labyrinthe); fclose(file); return NULL; }
    for (int i = 0; i < width; i++) {
        grid[i] = calloc(height, sizeof(struct LabyrintheNode*));
        if (!grid[i]) {
            for (int k = 0; k < i; k++) {
                free(grid[k]);
            }
            free(grid); free(labyrinthe); fclose(file); return NULL;
        }
    }

    // ------------------------------------------------------------------------------------
    // Lecture des nodes (Parsing)
    // ------------------------------------------------------------------------------------
    int x, y, cost; char typeStr[16];

    while (fgets(line, sizeof(line), file)) {
        cost = 1; // par défaut
        if (sscanf(line,
            " { \"x\": %d , \"y\": %d , \"type\": \"%15[^\"]\", \"cost\": %d\n",
            &x,&y, typeStr, &cost) == 4) {

            if (x < 0 || x >= width || y < 0 || y >= height) {
                fprintf(stderr, "WARNING: Node hors bornes ignoré x=%d y=%d\n", x, y);
                continue;
            }

            if (grid[x][y] != NULL) { // Soit doublons
                fprintf(stderr, "WARNING: Doublons ignoré x=%d y=%d\n", x, y);
            }

            // Type de noeud
            enum LabyrintheNodeType type = CELL;
            if (strcmp(typeStr, "WALL") == 0) type = WALL;
            else if (strcmp(typeStr, "START") == 0) type = START;
            else if (strcmp(typeStr, "END") == 0) type = END;
            else if (strcmp(typeStr, "CELL") != 0) { // Soit tout autre chose qui n'est pas dans une condition avant
                fprintf(stderr, "WARNING: type inconnu \"%s\" en x=%d y=%d\n, devient CELL\n", typeStr, x, y);
            }

            // travel_cost positif
            if (cost <= 0) {
                fprintf(stderr, "WARNING: travel_cost invalide (%d) en x=%d y=%d\n, corrigé\n", cost, x, y);
                cost = (type == WALL) ? 9999 : 1;
            }

            // Cohérence entre type et travel_cost
            if (type == WALL && cost < 9999) {
                fprintf(stderr, "WARNING: WALL avec cost=%d en x=%d y=%d\n", cost, x, y);
                cost = 9999;
            }
            if (type != WALL && cost >= 9999) {
                fprintf(stderr, "WARNING: %s avec cost=%d en x=%d y=%d\n", typeStr, cost, x, y);
                cost = 1;
            }

            struct LabyrintheNode *node = LabyrintheNode_CreateCoords(x,y, cost);
            if (!node) { fprintf(stderr, "ERREUR: allocation du Node: x=%d y=%d\n", x, y); continue; }
            node->type = type;
            grid[x][y] = node;

            if (type == START) { labyrinthe->firstNode = node; }
            }
    }

    // --------------------------------------------------------------------------------
    // Vérification: Cases manquantes
    // --------------------------------------------------------------------------------
    int countMissing = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (!grid[x][y]) {
                fprintf(stderr, "ERREUR: case manquante en x=%d y=%d\n", x, y);
                countMissing++;
            }
        }
    }
    if (countMissing > 0) {
        fprintf(stderr, "ERREUR: %d case(s) manquante(s)\n", countMissing);
        LOAD_CLEAN;
    }

    // --------------------------------------------------------------------------------
    // Vérification: FirstNode n'est pas un mur
    // --------------------------------------------------------------------------------
    if (labyrinthe->firstNode && labyrinthe->firstNode->type == WALL) {
        fprintf(stderr, "ERREUR: START est un mur\n");
        LOAD_CLEAN;
    }

    // --------------------------------------------------------------------------------
    // Vérification: 1 START et 1 END
    // --------------------------------------------------------------------------------
    int countStart = 0, countEnd = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (grid[x][y]->type == START) { countStart++; }
            if (grid[x][y]->type == END) { countEnd++; }
        }
    }
    if (countStart != 1) {
        fprintf(stderr, "ERREUR: %d START trouvé(s)", countStart);
        LOAD_CLEAN;
    }
    if (countEnd != 1) {
        fprintf(stderr, "ERREUR: %d END trouvé(s)", countEnd);
        LOAD_CLEAN;
    }

    // ------------------------------------------------------------------------------------
    // Relier voisins
    // ------------------------------------------------------------------------------------
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

    // Si aucun START trouvé (ne devrait pas arriver)
    if (!labyrinthe->firstNode) {
        fprintf(stderr, "ERREUR: aucun START trouvé\n");
        LOAD_CLEAN;
    }

    // ------------------------------------------------------------------------------------
    // Libération de la mémoire
    // ------------------------------------------------------------------------------------
    for (int x = 0; x < labyrinthe->width; x++) {
        free(grid[x]);
    }
    free(grid);

    fclose(file);
    return labyrinthe;
}