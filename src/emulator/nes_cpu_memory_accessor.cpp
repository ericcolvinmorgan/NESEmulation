#include <iostream>
#include <cstdint>
#include <cstring>
#include "../../include/emulator/nes_cpu_memory_accessor.h"

Byte NESCPUMemoryAccessor::ReadByte(uint16_t location, bool suppress_event)
{
    uint16_t readLocation;
    // https://wiki.nesdev.org/w/index.php/CPU_memory_map
    switch (location)
    {
    // $0000 - $1FFF is mirrored every $0800 bytes
    // 2KB internal RAM
    case 0x0000 ... 0x1FFF:
    {
        readLocation = location % 0x0800;
    }
    break;

    // $2000 - $3FFF is mirrored every $08 bytes
    // NES PPU registers
    case 0x2000 ... 0x3FFF:
    {
        readLocation = 0x2000 + ((location - 0x2000) % 0x08);
    }
    break;

    // NES APU and I/O registers
    case 0x4000 ... 0x4017:
    // APU and I/O functionality that is normally disabled
    case 0x4018 ... 0x401F:
    // Cartridge space: PRG ROM, PRG RAM, and mapper registers
    case 0x4020 ... 0xFFFF:
    default:
    {
        readLocation = location;
    }
    break;
    }
    const Byte value = memory_[readLocation];
    if(!suppress_event)
        OnMemoryRead(location);

    return value;
}

Word NESCPUMemoryAccessor::ReadWord(uint16_t location)
{
    return ReadByte(location) | (ReadByte(location + 1) << 8);
}

void NESCPUMemoryAccessor::WriteMemory(uint16_t location, Byte data, bool suppress_event)
{
    // https://wiki.nesdev.org/w/index.php/CPU_memory_map
    switch (location)
    {
    // $0000 - $1FFF is mirrored every $0800 bytes
    // 2KB internal RAM
    case 0x0000 ... 0x1FFF:
    {
        uint16_t ram_location = location % 0x0800;
        memory_[ram_location] = data;
    }
    break;

    // $2000 - $3FFF is mirrored every $08 bytes
    // NES PPU registers
    case 0x2000 ... 0x3FFF:
    {
        uint16_t ppu_location = 0x2000 + ((location - 0x2000) % 0x08);
        memory_[ppu_location] = data;
    }
    break;

    // NES APU and I/O registers
    case 0x4000 ... 0x4017:
    // APU and I/O functionality that is normally disabled
    case 0x4018 ... 0x401F:
    // Cartridge space: PRG ROM, PRG RAM, and mapper registers
    case 0x4020 ... 0xFFFF:
    {
        memory_[location] = data;
    }
    break;
    }

    if(!suppress_event)
        OnMemoryWrite(location);
}

void NESCPUMemoryAccessor::WriteMemory(uint16_t location, Word data, bool suppress_event)
{
    WriteMemory(location, (Byte)data, suppress_event);
    WriteMemory(location + 1, (Byte)(data >> 8), true);
}

void NESCPUMemoryAccessor::WriteMemory(uint16_t location, const Byte *data, uint16_t length)
{
    std::memcpy(&memory_[location], data, length);
}