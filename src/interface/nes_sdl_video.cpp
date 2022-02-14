#include <SDL2/SDL.h>
#include "../../include/interface/nes_sdl_video.h"

NESSDLVideo::~NESSDLVideo()
{
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyTexture(texture_);
    SDL_Quit();
}

void NESSDLVideo::RenderFrame()
{
    // Clear screen
    SDL_RenderClear(renderer_);

    // Load image at specified path
    int width = 256;
    int height = 240;

    uint8_t *screen = new uint8_t[width * height * 4];
    auto pixels = ppu_->GetScreenBuffer();
    SDL_Texture *screen_texture = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 256, 240);

    int palette[4] = {0x31, 0x16, 0x0d, 0x37};
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            Byte color = pixels[(h * width) + w];

            screen[(h * width * 4) + (w * 4)] = 1;
            screen[(h * width * 4) + (w * 4) + 1] = kColorMap[(palette[color] * 3) + 2];
            screen[(h * width * 4) + (w * 4) + 2] = kColorMap[(palette[color] * 3) + 1];
            screen[(h * width * 4) + (w * 4) + 3] = kColorMap[(palette[color] * 3) + 0];
        }
    }

    SDL_Rect table_1_viewport;
    table_1_viewport.x = 0;
    table_1_viewport.y = 0;
    table_1_viewport.w = 256;
    table_1_viewport.h = 240;
    SDL_RenderSetViewport(renderer_, &table_1_viewport);
    SDL_UpdateTexture(screen_texture, NULL, screen, width * 4);
    SDL_RenderCopy(renderer_, screen_texture, NULL, NULL);

    delete[] screen;
    SDL_DestroyTexture(screen_texture);

    // // Render texture to screen
    // SDL_RenderCopy(renderer_, texture_, NULL, NULL);

    // Update screen
    SDL_RenderPresent(renderer_);
}

void NESSDLVideo::RenderPalette()
{
    // Clear screen
    SDL_RenderClear(renderer_);

    // Load image at specified path
    int width = 128;
    int height = 128;

    uint8_t *table1 = new uint8_t[width * height * 4];
    SDL_Texture *table1_texture = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 128, 128);
    uint8_t *table2 = new uint8_t[width * height * 4];
    SDL_Texture *table2_texture = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 128, 128);

    for (int h = 0; h < height; h++)
    {
        int h_tile = h / 8;
        int h_byte = h % 8;
        for (int w = 0; w < width; w++)
        {
            int w_tile = w / 8;
            int w_bit = w % 8;

            Word location = (h_tile << 8) | (w_tile << 4) | h_byte;

            uint8_t top_byte_1 = ppu_memory_->ReadByte(location + 0x0000);
            uint8_t bottom_byte_1 = ppu_memory_->ReadByte(location + 8 + 0x0000);
            uint8_t color_1 = (((top_byte_1 >> (7 - w_bit)) & 0b00000001) << 1) + ((bottom_byte_1 >> (7 - w_bit)) & 0b00000001);
            int palette[4] = {0x31, 0x16, 0x0d, 0x37};

            table1[(h * width * 4) + (w * 4)] = 1;
            table1[(h * width * 4) + (w * 4) + 1] = kColorMap[(palette[color_1] * 3) + 2];
            table1[(h * width * 4) + (w * 4) + 2] = kColorMap[(palette[color_1] * 3) + 1];
            table1[(h * width * 4) + (w * 4) + 3] = kColorMap[(palette[color_1] * 3) + 0];

            uint8_t top_byte_2 = ppu_memory_->ReadByte(location + 0x1000);
            uint8_t bottom_byte_2 = ppu_memory_->ReadByte(location + 8 + 0x1000);
            uint8_t color_2 = (((top_byte_2 >> (7 - w_bit)) & 0b00000001) << 1) + ((bottom_byte_2 >> (7 - w_bit)) & 0b00000001);

            table2[(h * width * 4) + (w * 4)] = 1;
            table2[(h * width * 4) + (w * 4) + 1] = kColorMap[(palette[color_2] * 3) + 2];
            table2[(h * width * 4) + (w * 4) + 2] = kColorMap[(palette[color_2] * 3) + 1];
            table2[(h * width * 4) + (w * 4) + 3] = kColorMap[(palette[color_2] * 3) + 0];
        }
    }

    SDL_Rect table_1_viewport;
    table_1_viewport.x = 0;
    table_1_viewport.y = 0;
    table_1_viewport.w = 512;
    table_1_viewport.h = 512;
    SDL_RenderSetViewport(renderer_, &table_1_viewport);
    SDL_UpdateTexture(table1_texture, NULL, table1, width * 4);
    SDL_RenderCopy(renderer_, table1_texture, NULL, NULL);

    SDL_Rect table_2_viewport;
    table_2_viewport.x = 512;
    table_2_viewport.y = 0;
    table_2_viewport.w = 512;
    table_2_viewport.h = 512;
    SDL_RenderSetViewport(renderer_, &table_2_viewport);
    SDL_UpdateTexture(table2_texture, NULL, table2, width * 4);
    SDL_RenderCopy(renderer_, table2_texture, NULL, NULL);

    delete[] table1;
    delete[] table2;
    SDL_DestroyTexture(table1_texture);
    SDL_DestroyTexture(table2_texture);

    // // Render texture to screen
    // SDL_RenderCopy(renderer_, texture_, NULL, NULL);

    // Update screen
    SDL_RenderPresent(renderer_);
}

void NESSDLVideo::InitVideo()
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
                texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, kScreenWidth, kScreenHeight);
            }
        }
    }
}

void NESSDLVideo::GetInput() {}