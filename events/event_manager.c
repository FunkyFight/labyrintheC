#include "event_manager.h"
#include "quit_event.h"
#include "keyboard_events.h"

bool event_manager_emit_event(struct Game *g) {
    SDL_EventType type = g->event->type;

    switch(type) {
        default:
            return false;

        case SDL_EVENT_QUIT:
            event_on_quit(g);
            return true;

        case SDL_EVENT_KEY_DOWN:
            event_on_key_down(g);
            return true;
    }
}
