#pragma once
#include "memory_accessor_interface.h"

class RawMemoryAccessor : public MemoryAccessorInterface
{
private:
    Byte memory_[0xffff + 1] = {0};

public:
    RawMemoryAccessor() {}
    ~RawMemoryAccessor() {}

    Byte ReadByte(uint16_t location, bool suppress_event = false);
    Word ReadWord(uint16_t location);
    void WriteMemory(uint16_t location, Byte data, bool suppress_event = false);
    void WriteMemory(uint16_t location, Word data, bool suppress_event = false);
    void WriteMemory(uint16_t location, const Byte *data, uint16_t length);
};