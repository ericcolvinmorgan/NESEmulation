#pragma once
#include "../emulator/video_interface.h"

class SDLVideo : public VideoInterface
{
private:
    const int kScreenWidth = 256;
    const int kScreenHeight = 256;
    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
    SDL_Texture *texture_ = nullptr;
    MemoryAccessorInterface *memory_ = nullptr;

public:
    SDLVideo(MemoryAccessorInterface *memory) : VideoInterface() { memory_ = memory; }
    ~SDLVideo();
    void InitVideo();
    void RenderFrame();
    void GetInput();
};