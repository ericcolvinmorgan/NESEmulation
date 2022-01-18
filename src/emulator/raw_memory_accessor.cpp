#include <iostream>
#include <cstdint>
#include <cstring>
#include "../../include/emulator/raw_memory_accessor.h"

BYTE RawMemoryAccessor::readByte(uint16_t location)
{
    return _memory[location];
}

WORD RawMemoryAccessor::readWord(uint16_t location)
{
    return _memory[location] | ((_memory[location + 1]) << 8);
}

void RawMemoryAccessor::writeMemory(uint16_t location, BYTE data)
{
    _memory[location] = data;
}

void RawMemoryAccessor::writeMemory(uint16_t location, WORD data)
{
    *((WORD*)(_memory + location)) = data;
}

void RawMemoryAccessor::writeMemory(uint16_t location, const BYTE *data, uint16_t length)
{
    std::memcpy(&_memory[location], data, length);
}