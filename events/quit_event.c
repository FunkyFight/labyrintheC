//
// Created by Funky on 15/12/2025.
//

#include <stdio.h>
#include "quit_event.h"

void event_on_quit(struct Game *g) {

    g->is_running = false;
    printf("Have a nice day!");

}