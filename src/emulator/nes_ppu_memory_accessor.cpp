#include <iostream>
#include <cstdint>
#include <cstring>
#include "../../include/emulator/nes_ppu_memory_accessor.h"

Byte NESPPUMemoryAccessor::ReadByte(uint16_t location)
{
    // https://wiki.nesdev.org/w/index.php/PPU_memory_map
    switch (location)
    {
    // Pattern Table 0
    case 0x0000 ... 0x0fff:
    // Pattern Table 1
    case 0x1000 ... 0x1fff:
    // Nametable 0
    case 0x2000 ... 0x23ff:
    // Nametable 1
    case 0x2400 ... 0x27ff:
    // Nametable 2
    case 0x2800 ... 0x2bff:
    // Nametable 3
    case 0x2c00 ... 0x2fff:
    {
        return memory_[location];
    }
    break;

    // Mirrors of $2000-$2EFF
    case 0x3000 ... 0x3eff:
    {
        uint16_t ppu_location = (location - 0x1000);
        return memory_[ppu_location];
    }
    break;

    // Mirrors of $2000-$2EFF
    case 0x3f00 ... 0x3f1f:
    {
        return memory_[location];
    }
    break;

    // Mirrors of $3F00-$3F1F
    case 0x3f20 ... 0x3fff:
    {
        uint16_t ppu_location = 0x3f20 + ((location - 0x3f20) % 0x20);
        return memory_[ppu_location];
    }
    break;
    }
    return memory_[location];
}

Word NESPPUMemoryAccessor::ReadWord(uint16_t location)
{
    return ReadByte(location) | (ReadByte(location + 1) << 8);
}

void NESPPUMemoryAccessor::WriteMemory(uint16_t location, Byte data, bool suppress_event)
{
    // https://wiki.nesdev.org/w/index.php/PPU_memory_map
    switch (location)
    {
    // Pattern Table 0
    case 0x0000 ... 0x0fff:
    // Pattern Table 1
    case 0x1000 ... 0x1fff:
    // Nametable 0
    case 0x2000 ... 0x23ff:
    // Nametable 1
    case 0x2400 ... 0x27ff:
    // Nametable 2
    case 0x2800 ... 0x2bff:
    // Nametable 3
    case 0x2c00 ... 0x2fff:
    {
        memory_[location] = data;
    }
    break;

    // Mirrors of $2000-$2EFF
    case 0x3000 ... 0x3eff:
    {
        uint16_t ppu_location = (location - 0x1000);
        memory_[ppu_location] = data;
    }
    break;

    // Palette RAM indexes
    case 0x3f00 ... 0x3f1f:
    {
        memory_[location] = data;
    }
    break;

    // Mirrors of $3F00-$3F1F
    case 0x3f20 ... 0x3fff:
    {
        uint16_t ppu_location = 0x3f20 + ((location - 0x3f20) % 0x20);
        memory_[ppu_location] = data;
    }
    break;
    }
}

void NESPPUMemoryAccessor::WriteMemory(uint16_t location, Word data, bool suppress_event)
{
    WriteMemory(location, (Byte)data);
    WriteMemory(location + 1, (Byte)(data >> 8));
}

void NESPPUMemoryAccessor::WriteMemory(uint16_t location, const Byte *data, uint16_t length)
{
    std::memcpy(&memory_[location], data, length);
}