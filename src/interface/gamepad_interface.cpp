#include "../../include/interface/gamepad_interface.h"

uint8_t GamepadInterface::GetInputState() const
{
    uint8_t buttonStates = 0;
    if (SDL_GameControllerGetButton(gameController_, SDL_CONTROLLER_BUTTON_A)) { // A
        buttonStates |= 1;
    }
    if (SDL_GameControllerGetButton(gameController_, SDL_CONTROLLER_BUTTON_B)){ // B
        buttonStates |= (1 << 1);
    }
    if (SDL_GameControllerGetButton(gameController_, SDL_CONTROLLER_BUTTON_BACK)){ // Select
        buttonStates |= (1 << 2);
    }
    if (SDL_GameControllerGetButton(gameController_, SDL_CONTROLLER_BUTTON_START)){ // Start
        buttonStates |= (1 << 3);
    }
    if (SDL_GameControllerGetButton(gameController_, SDL_CONTROLLER_BUTTON_DPAD_UP)) { // Up
        buttonStates |= (1 << 4);
    }
    if (SDL_GameControllerGetButton(gameController_, SDL_CONTROLLER_BUTTON_DPAD_DOWN)){ // Down
        buttonStates |= (1 << 5);
    }
    if (SDL_GameControllerGetButton(gameController_, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) { // Left
        buttonStates |= (1 << 6);
    }
    if (SDL_GameControllerGetButton(gameController_, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)){ // Right
        buttonStates |= (1 << 7);
    }

    return buttonStates;
}
