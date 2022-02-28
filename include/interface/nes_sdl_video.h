#pragma once
#include "../emulator/video_interface.h"
#include "../emulator/ppu.h"

class NESSDLVideo : public VideoInterface
{
private:
    // const int kScreenWidth = 1024;
    // const int kScreenHeight = 512;
    const int kScreenWidth = 512;
    const int kScreenHeight = 480;
    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
    SDL_Texture *texture_ = nullptr;
    PPU *ppu_ = nullptr;
    MemoryAccessorInterface *ppu_memory_ = nullptr;

public:
    NESSDLVideo(PPU *ppu, MemoryAccessorInterface *ppu_memory) : VideoInterface()
    {
        ppu_ = ppu;
        ppu_memory_ = ppu_memory;
    }
    
    ~NESSDLVideo();
    void InitVideo();
    void RenderFrame();
    void RenderPalette();
    void GetInput();
};