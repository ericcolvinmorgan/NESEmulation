#include <iostream>
#include <cstdint>
#include <cstring>
#include "../../include/emulator/raw_memory_accessor.h"

Byte RawMemoryAccessor::ReadByte(uint16_t location)
{
    return memory_[location];
}

Word RawMemoryAccessor::ReadWord(uint16_t location)
{
    return memory_[location] | ((memory_[location + 1]) << 8);
}

void RawMemoryAccessor::WriteMemory(uint16_t location, Byte data)
{
    memory_[location] = data;
}

void RawMemoryAccessor::WriteMemory(uint16_t location, Word data)
{
    *((Word*)(memory_ + location)) = data;
}

void RawMemoryAccessor::WriteMemory(uint16_t location, const Byte *data, uint16_t length)
{
    std::memcpy(&memory_[location], data, length);
}