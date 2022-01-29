#pragma once
#include "memory_accessor_interface.h"

class ControllerInterface
{
protected:
    MemoryAccessorInterface* memory_;

public:
    ControllerInterface(MemoryAccessorInterface* memory) { memory_ = memory; };
    virtual ~ControllerInterface() {};
    virtual void InitController() = 0;
    virtual void WriteInput(Byte address) = 0;
    virtual void ReadOutput(Byte address) = 0;
};