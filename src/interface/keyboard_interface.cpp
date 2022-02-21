#include "../../include/interface/keyboard_interface.h"
#include <SDL2/SDL.h>

uint8_t KeyboardInterface::GetInputState() const
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        /* 
            Poll all events. These will be handled by filters created elsewhere.
            From https://wiki.libsdl.org/SDL_GetKeyboardState:
            This function gives you the current state after all events have been processed, 
            so if a key or button has been pressed and released before you process events, 
            then the pressed state will never show up in the SDL_GetKeyboardState() calls.
        */
    }

    // SDL owns this state so we shouldn't free it
    const uint8_t *state = SDL_GetKeyboardState(NULL);
    uint8_t buttonStates = 0;
    if (state[SDL_SCANCODE_J])
    { // A
        buttonStates |= 1;
    }
    if (state[SDL_SCANCODE_K])
    { // B
        buttonStates |= (1 << 1);
    }
    if (state[SDL_SCANCODE_SPACE])
    { // Select
        buttonStates |= (1 << 2);
    }
    if (state[SDL_SCANCODE_RETURN])
    { // Start
        buttonStates |= (1 << 3);
    }
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
    { // Up
        buttonStates |= (1 << 4);
    }
    if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
    { // Down
        buttonStates |= (1 << 5);
    }
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
    { // Left
        buttonStates |= (1 << 6);
    }
    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
    { // Right
        buttonStates |= (1 << 7);
    }

    return buttonStates;
}
