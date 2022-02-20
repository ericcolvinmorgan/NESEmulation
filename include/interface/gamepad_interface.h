#pragma once
#include <SDL2/SDL.h>
#include "../emulator/input_interface.h"

class GamepadInterface : public InputInterface
{
    private:
        SDL_GameController* gameController_ = nullptr;

    public:
        // Caller maintains ownership of SDL_GameController
        GamepadInterface(SDL_GameController* sdlController): gameController_(sdlController) {};
        uint8_t GetInputState() const override;
};