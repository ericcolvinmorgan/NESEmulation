#pragma once
#include "memory_accessor_interface.h"

class NESCPUMemoryAccessor : public MemoryAccessorInterface
{
private:
    Byte memory_[0xffff + 1] = {0};

public:
    NESCPUMemoryAccessor() {}

    Byte ReadByte(uint16_t location);
    Word ReadWord(uint16_t location);
    void WriteMemory(uint16_t location, Byte data);
    void WriteMemory(uint16_t location, Word data);
    void WriteMemory(uint16_t location, const Byte *data, uint16_t length);
};