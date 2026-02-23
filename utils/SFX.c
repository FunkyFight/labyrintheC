//
// Created by Funky on 20/02/2026.
//

#include <stdlib.h>
#include <stdio.h>
#include "SFX.h"

#define MAX_SOUNDS 20

typedef struct {
    SDL_AudioStream *stream;
    unsigned int playOrder;
} ActiveSound;

static ActiveSound activeSounds[MAX_SOUNDS] = {{NULL, 0}};
static unsigned int soundCounter = 0;

void SFX_RandomGenSound()
{
    struct Sound* sounds[] = {
        &(struct Sound){.name = "assets/sound/gen/7.wav"},
        &(struct Sound){.name = "assets/sound/gen/8.wav"},
        &(struct Sound){.name = "assets/sound/gen/9.wav"},
        &(struct Sound){.name = "assets/sound/gen/10.wav"},
        &(struct Sound){.name = "assets/sound/gen/11.wav"},
        &(struct Sound){.name = "assets/sound/gen/12.wav"},
    };

    int randomSound = rand() % 6;

    SFX_PlaySound(sounds[randomSound]->name);
}

void SFX_CleanupFinishedStreams() {
    for(int i = 0; i < MAX_SOUNDS; i++) {
        if(activeSounds[i].stream != NULL) {
            int queued = SDL_GetAudioStreamQueued(activeSounds[i].stream);
            if(queued == 0) {
                SDL_DestroyAudioStream(activeSounds[i].stream);
                activeSounds[i].stream = NULL;
                activeSounds[i].playOrder = 0;
            }
        }
    }
}

void SFX_PlaySound(const char* filename) {
    SFX_CleanupFinishedStreams();

    int slot = -1;
    for(int i = 0; i < MAX_SOUNDS; i++) {
        if(activeSounds[i].stream == NULL) {
            slot = i;
            break;
        }
    }

    // Si pas de slot disponible, couper le son le plus vieux
    if(slot == -1) {
        unsigned int oldestOrder = soundCounter;
        int oldestSlot = 0;

        for(int i = 0; i < MAX_SOUNDS; i++) {
            if(activeSounds[i].playOrder < oldestOrder) {
                oldestOrder = activeSounds[i].playOrder;
                oldestSlot = i;
            }
        }

        SDL_DestroyAudioStream(activeSounds[oldestSlot].stream);
        activeSounds[oldestSlot].stream = NULL;
        slot = oldestSlot;
    }

    SDL_AudioSpec spec;
    Uint8 *data;
    Uint32 len;

    if (SDL_LoadWAV(filename, &spec, &data, &len)) {
        SDL_AudioStream *stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);

        if (stream) {
            // Baisser le volume à 30%
            SDL_SetAudioStreamGain(stream, 0.3f);

            if(SDL_PutAudioStreamData(stream, data, len)) {
                SDL_FlushAudioStream(stream);
                SDL_ResumeAudioStreamDevice(stream);
                activeSounds[slot].stream = stream;
                activeSounds[slot].playOrder = soundCounter++;
            } else {
                SDL_DestroyAudioStream(stream);
            }
        }

        SDL_free(data);
    }
}