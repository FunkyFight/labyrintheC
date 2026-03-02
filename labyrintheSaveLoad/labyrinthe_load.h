#ifndef LABYRINTHE_LOAD_H
#define LABYRINTHE_LOAD_H

// Charge un labyrinthe depuis un fichier JSON dans une grille 2D
// file_path : chemin vers le fichier JSON ici dans data

struct Labyrinthe;
struct Labyrinthe* Labyrinthe_LoadJSON(const char *file_path);

#endif
