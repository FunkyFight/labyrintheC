#include "event_manager.h"
#include "quit_event.h"

bool event_manager_emit_event(struct Game *g) {
    SDL_EventType type = g->event->type;

    switch(type) {
        default:
            break;

        case SDL_EVENT_QUIT:
            event_on_quit(g);
            break;
    }
}
