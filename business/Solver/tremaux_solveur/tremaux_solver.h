//
// Created by rapha on 22/02/2026.
//

#ifndef LABYRINTHE_TREMAUX_SOLVER_H
#define LABYRINTHE_TREMAUX_SOLVER_H
#include <stdbool.h>

struct RightSolverData {
    struct ListNode* pathSolved;
    bool isPathSolvedListNodeCreated;
    int solverDirection; // 0 = Nord ; 1 = East ; 2 = South ; 3 = West
    struct LabyrintheNode* node;
};
#endif //LABYRINTHE_TREMAUX_SOLVER_H