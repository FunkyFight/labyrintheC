//
// Created by rapha on 17/02/2026.
//

#ifndef LABYRINTHE_RANDOM_SOLVER_H
#define LABYRINTHE_RANDOM_SOLVER_H
#include <stdbool.h>

struct RondomSolverData {
    struct ListNode* pathSolved;
    bool isPathSolvedListNodeCreated;
    struct LabyrintheNode* node;
};

#endif //LABYRINTHE_RANDOM_SOLVER_H