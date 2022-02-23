#include <iostream>
#include <cstdint>
#include <cstring>
#include "../../include/emulator/raw_memory_accessor.h"

Byte RawMemoryAccessor::ReadByte(uint16_t location, bool suppress_event)
{
    const Byte value = memory_[location];
    if(!suppress_event)
        OnMemoryRead(location);
    return value;
}

Word RawMemoryAccessor::ReadWord(uint16_t location)
{
    return memory_[location] | ((memory_[location + 1]) << 8);
}

void RawMemoryAccessor::WriteMemory(uint16_t location, Byte data, bool suppress_event)
{
    memory_[location] = data;
    if(!suppress_event)
        OnMemoryWrite(location);
}

void RawMemoryAccessor::WriteMemory(uint16_t location, Word data, bool suppress_event)
{
    *((Word *)(memory_ + location)) = data;
    if(!suppress_event)
        OnMemoryWrite(location);
}

void RawMemoryAccessor::WriteMemory(uint16_t location, const Byte *data, uint16_t length)
{
    std::memcpy(&memory_[location], data, length);
}