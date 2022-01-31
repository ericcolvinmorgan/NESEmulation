#include <SDL2/SDL.h>
#include "../../include/interface/demo_controller.h"

void DemoController::WriteInput(Byte address)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
        {
            memory_->WriteMemory(address, (uint8_t)event.key.keysym.sym);
        }
    }
}