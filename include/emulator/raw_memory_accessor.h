#pragma once
#include "imemory_accessor.h"

class RawMemoryAccessor : public IMemoryAccessor
{
private:
    BYTE _memory[0xffff + 1] = {0};

public:
    RawMemoryAccessor() {}

    BYTE readByte(uint16_t location);
    WORD readWord(uint16_t location);
    void writeMemory(uint16_t location, BYTE data);
    void writeMemory(uint16_t location, WORD data);
    void writeMemory(uint16_t location, BYTE *data, uint16_t length);
};