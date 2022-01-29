#pragma once
#include "memory_accessor_interface.h"

class VideoInterface
{
protected:
    MemoryAccessorInterface* memory_;

public:
    VideoInterface(MemoryAccessorInterface* memory) { memory_ = memory; };
    virtual ~VideoInterface() {};
    virtual void InitVideo() = 0;
    virtual void RenderFrame() = 0;
    virtual void GetInput() = 0;
};