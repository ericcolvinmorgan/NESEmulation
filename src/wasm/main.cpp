// FROM https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm
// AND https://emscripten.org/docs/getting_started/Tutorial.html

#include <stdio.h>
#include <SDL2/SDL.h>
#include <emscripten/emscripten.h>
#include "../../include/emulator/emulator.h"

const int kScreenWidth = 256;
const int kScreenHeight = 224;

Uint8 r = 0x00;
Uint8 g = 0x00;
Uint8 b = 0xFF;

SDL_Window *window = nullptr;
Emulator *emulator =nullptr;

void RenderContent()
{
    emulator->AdvanceFrame();
    SDL_Surface *screen_surface = SDL_GetWindowSurface(window);
    Uint8 temp = b;
    b = g;
    g = r;
    r = temp;
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, r, g, b));
    SDL_UpdateWindowSurface(window);
    SDL_Delay(1000);
    printf("Current Frame: %d\n", emulator->GetFrame());
}

int main()
{
    // Initialize
    emulator = new Emulator();
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, kScreenWidth, kScreenHeight, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            emscripten_set_main_loop(RenderContent, 0, 1);
        }
    }

    // Clean-up
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}