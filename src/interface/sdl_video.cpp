#include <SDL2/SDL.h>
#include "../../include/interface/sdl_video.h"

SDLVideo::~SDLVideo()
{
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyTexture(texture_);
    SDL_Quit();
}

void SDLVideo::RenderFrame()
{
    // Clear screen
    SDL_RenderClear(renderer_);

    // Load image at specified path
    int width = kScreenWidth / 8;
    int height = kScreenHeight / 8;

    uint8_t *screen = new uint8_t[width * height * 4];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            uint8_t color = memory_->ReadByte(0x0200 + (width * h) + w);
            screen[(h * width * 4) + (w * 4)] = 1;
            screen[(h * width * 4) + (w * 4) + 1] = kColorMap[(color * 3) + 2];
            screen[(h * width * 4) + (w * 4) + 2] = kColorMap[(color * 3) + 1];
            screen[(h * width * 4) + (w * 4) + 3] = kColorMap[(color * 3) + 0];
        }
    }

    SDL_UpdateTexture(texture_, NULL, screen, width * 4);
    delete[] screen;

    // Render texture to screen
    SDL_RenderCopy(renderer_, texture_, NULL, NULL);

    // Update screen
    SDL_RenderPresent(renderer_);
}

void SDLVideo::InitVideo()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        // Set texture filtering to linear
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

        window_ = SDL_CreateWindow("NES Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, kScreenWidth, kScreenHeight, SDL_WINDOW_SHOWN);
        if (window_ == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            // Create renderer for window
            renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
            if (renderer_ == NULL)
            {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 1);

                // Create Texture
                texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, kScreenWidth / 8, kScreenHeight / 8);
            }
        }
    }
}

void SDLVideo::GetInput() {}