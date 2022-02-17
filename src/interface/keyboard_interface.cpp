#include "../../include/interface/keyboard_interface.h"
#include <SDL2/SDL.h>

uint8_t KeyboardInterface::GetInputState() const
{
    // SDL owns this state so we shouldn't free it
    const uint8_t *state = SDL_GetKeyboardState(NULL);
    uint8_t buttonStates = 0;
    if (state[SDL_SCANCODE_J]) { // A
        buttonStates |= 1;
    }
    if (state[SDL_SCANCODE_K]){ // B
        buttonStates |= (1 << 1);
    }
    if (state[SDL_SCANCODE_SPACE]){ // Select
        buttonStates |= (1 << 2);
    }
    if (state[SDL_SCANCODE_RETURN]){ // Start
        buttonStates |= (1 << 3);
    }
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) { // Up
        buttonStates |= (1 << 4);
    }
    if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]){ // Down
        buttonStates |= (1 << 5);
    }
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) { // Left
        buttonStates |= (1 << 6);
    }
    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]){ // Right
        buttonStates |= (1 << 7);
    }

    return buttonStates;
}
