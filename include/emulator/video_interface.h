#pragma once
#include "memory_accessor_interface.h"

class VideoInterface
{
public:
    VideoInterface() {};
    virtual ~VideoInterface() {};
    virtual void InitVideo() = 0;
    virtual void RenderFrame() = 0;
    virtual void GetInput() = 0;
};