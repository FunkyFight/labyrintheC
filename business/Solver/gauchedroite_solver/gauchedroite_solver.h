//
// Created by rapha on 26/02/2026.
//

#ifndef LABYRINTHE_TES_SOLVER_H
#define LABYRINTHE_TES_SOLVER_H
#include <stdbool.h>

struct GaucheDroiteSolverData {
    struct ListNode* pathSolved;
    bool isPathSolvedListNodeCreated;
    bool droiteGauche;
    int solverDirection; // 0 = Nord ; 1 = East ; 2 = South ; 3 = West
    struct LabyrintheNode* node;
};

#endif //LABYRINTHE_TES_SOLVER_H