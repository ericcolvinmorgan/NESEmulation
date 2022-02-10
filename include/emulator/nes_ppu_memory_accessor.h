#pragma once
#include "memory_accessor_interface.h"

class NESPPUMemoryAccessor : public MemoryAccessorInterface
{
private:
    Byte memory_[0x3f1f + 1] = {0};

public:
    NESPPUMemoryAccessor() {}
    ~NESPPUMemoryAccessor() {}

    Byte ReadByte(uint16_t location);
    Word ReadWord(uint16_t location);
    void WriteMemory(uint16_t location, Byte data, bool suppress_event = false);
    void WriteMemory(uint16_t location, Word data, bool suppress_event = false);
    void WriteMemory(uint16_t location, const Byte *data, uint16_t length);
};