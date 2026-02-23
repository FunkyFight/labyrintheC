//
// Created by rapha on 18/02/2026.
//

#ifndef LABYRINTHE_PLEDGE_SOLVER_H
#define LABYRINTHE_PLEDGE_SOLVER_H
#include <stdbool.h>


struct PledgeSolverData {
    struct ListNode* pathSolved;
    bool isPathSolvedListNodeCreated;
    int solverDirection; // 0 = Nord ; 1 = East ; 2 = South ; 3 = West
    int discision;
    struct LabyrintheNode* node;
};


#endif //LABYRINTHE_PLEDGE_SOLVER_H