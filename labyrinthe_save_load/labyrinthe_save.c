#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "labyrinthe_save.h"
#include "../types.h"
#include "../business/nodes/labyrinthe_node.h"

void Labyrinthe_SaveJSON(struct Labyrinthe *labyrinthe) {

    if (!labyrinthe || !labyrinthe->firstNode) return;

    static const char *SAVE_FILE_PATH = "../data/labyrinthe.json";
    //Ouverture du fichier en mode écriture
    FILE *file = fopen(SAVE_FILE_PATH, "w");
    if (!file) {
        perror("Erreur ouverture fichier");
        return;
    }

    int width = labyrinthe->width, height = labyrinthe->height;
    // Informations sur le Labirynthe
    fprintf(file, "{\n  \"width\": %d,\n  \"height\": %d,\n  \"nodes\": [\n", width, height);

    //Création d'une grille temporaire
    struct LabyrintheNode ***grid = malloc(width * sizeof(struct LabyrintheNode**));
    if (!grid) {fclose(file); printf("ERREUR: Création de la grille"); return;}
    for (int x = 0; x < width; x++) {
        grid[x] = calloc(height, sizeof(struct LabyrintheNode*));
        if (!grid[x]) {
            for (int k = 0; k < x; k++) free(grid[k]);
            {
                free(grid); fclose(file); return;
            }
        }
    }

    //remplissage par parcours en largeur (BFS)
    int maxNodes = width * height;
    struct LabyrintheNode **queue = malloc(maxNodes * sizeof(struct LabyrintheNode*));
    if (!queue) {
        for (int k = 0; k < width; k++) { free(grid[k]); }
        free(grid); fclose(file); fprintf(stderr, "ERREUR: queue\n"); return;
    }
    int front = 0, back = 0;

    struct LabyrintheNode *start = labyrinthe->firstNode;
    if (start->x < 0 || start->x >= width || start->y < 0 || start->y >= height) {
        printf("ERREUR: Vérification des coordonnéesx=%d y=%d\n", start->x, start->y);
        for (int k = 0; k < width; k++) { free(grid[k]); }
        free(grid); free(queue); fclose(file); return;
    }

    grid[start->x][start->y] = start;
    queue[back++] = start;

    while (front < back) {
        struct LabyrintheNode *current = queue[front++];
        if (!current) continue;

        struct LabyrintheNode *voisins[4] = {
            current->north,
            current->south,
            current->east,
            current->west
        };

        for (int i = 0; i < 4; i++) {
            struct LabyrintheNode *n = voisins[i];
            if (!n) continue;
            if (n->x < 0 || n->x >= width || n->y < 0 || n->y >= height) { continue; }
            if (grid[n->x][n->y] == NULL) {
                grid[n->x][n->y] = n;
                queue[back++] = n;
            }

        }
    }
    free(queue);

    //Ecriture
    bool firstNodeWritten = false;

    int placeholderCount = 0;
    struct LabyrintheNode **placeholders = malloc(width * height * sizeof(struct LabyrintheNode*));

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            struct LabyrintheNode *node = grid[x][y];
            if (!node) {
                node = LabyrintheNode_CreateCoords(x,y,9999);
                if (!node) {
                    fprintf(stderr, "ERREUR: allocation node placeholder %dx%d",x,y);
                    for (int k = 0; k < placeholderCount; k++) { if (placeholders[k]) { free(placeholders[k]); } }
                    free(placeholders);
                    for (int k = 0; k < width; k++) { free(grid[k]); }
                    free(grid); fclose(file); return;
                }
                grid[x][y] = node;
                if (placeholders) { placeholders[placeholderCount++] = node; }
            }

            char *typeStr;
            switch(node->type) {
                case WALL: typeStr = "WALL"; break;
                case START: typeStr = "START"; break;
                case END: typeStr = "END"; break;
                default: typeStr = "CELL"; break;
            }

            if (firstNodeWritten) fprintf(file, ",\n");
            fprintf(file, "    { \"x\": %d, \"y\": %d, \"type\": \"%s\" }", x, y, typeStr);
            firstNodeWritten = true;
        }
    }

    fprintf(file, "\n  ]\n}\n");

    for (int k=0; k < placeholderCount; k++) { if (placeholders[k]) { free(placeholders[k]); }}
    free(placeholders);
    for (int x=0; x < width; x++) { free(grid[x]); }
    free(grid);
    fclose(file);
}