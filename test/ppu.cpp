#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/constants.h"
#include "../include/emulator/ppu.h"
#include "../include/emulator/nes_ppu_memory_accessor.h"
#include "../include/emulator/nes_cpu_memory_accessor.h"

TEST_CASE("PPU - Write to PPUADDR")
{
    NESPPUMemoryAccessor ppu_memory;
    NESCPUMemoryAccessor cpu_memory;
    PPU ppu(&ppu_memory, &cpu_memory);

    cpu_memory.WriteMemory(0x2006, (Byte)0x06);
    cpu_memory.WriteMemory(0x2006, (Byte)0x00);

    REQUIRE(ppu.getVram() == 0x0600);
}

TEST_CASE("PPU - Write to PPUADDR with mirroring down if address is over 3fff")
{
    NESPPUMemoryAccessor ppu_memory;
    NESCPUMemoryAccessor cpu_memory;
    PPU ppu(&ppu_memory, &cpu_memory);

    cpu_memory.WriteMemory(0x2006, (Byte)0x4f);
    cpu_memory.WriteMemory(0x2006, (Byte)0xff);

    REQUIRE(ppu.getVram() == 0x0fff);
}

TEST_CASE("PPU - Write to PPUADDR, carry over vram until both writes completed")
{
    NESPPUMemoryAccessor ppu_memory;
    NESCPUMemoryAccessor cpu_memory;
    PPU ppu(&ppu_memory, &cpu_memory);

    cpu_memory.WriteMemory(0x2006, (Byte)0x4f);
    cpu_memory.WriteMemory(0x2006, (Byte)0xff);

    REQUIRE(ppu.getVram() == 0x0fff);

    cpu_memory.WriteMemory(0x2006, (Byte)0x12);

    REQUIRE(ppu.getVram() == 0x0fff); // vram should not be updated yet

    cpu_memory.WriteMemory(0x2006, (Byte)0x34);

    REQUIRE(ppu.getVram() == 0x1234); // vram should now be oupdated
}

TEST_CASE("PPU - Read from PPUDATA - Bit 2 of PPUCTRL Set")
{
    NESPPUMemoryAccessor ppu_memory;
    NESCPUMemoryAccessor cpu_memory;
    ppu_memory.WriteMemory(0xfff, (Byte)0x33);
    PPU ppu(&ppu_memory, &cpu_memory);

    cpu_memory.WriteMemory(0x2006, (Byte)0x4f);
    cpu_memory.WriteMemory(0x2006, (Byte)0xff);

    ppu.HandlePPUDATARead(); // dummy read, vram++

    REQUIRE(ppu.getVram() == 0x0fff + 1);
    REQUIRE(ppu.HandlePPUDATARead() == 0x33); // data read, vram++
    REQUIRE(ppu.getVram() == 0x0fff + 2);
}

TEST_CASE("PPU - Read from PPUDATA - Bit 2 of PPUCTRL Cleared")
{
    NESPPUMemoryAccessor ppu_memory;
    NESCPUMemoryAccessor cpu_memory;
    ppu_memory.WriteMemory(0xfff, (Byte)0x33);
    PPU ppu(&ppu_memory, &cpu_memory);

    ppu.setPPUCTRLData(0b00000100);
    
    cpu_memory.WriteMemory(0x2006, (Byte)0x4f);
    cpu_memory.WriteMemory(0x2006, (Byte)0xff);

    ppu.HandlePPUDATARead(); // dummy read, vram++

    REQUIRE(ppu.getVram() == 0x0fff + 32);
    REQUIRE(ppu.HandlePPUDATARead() == 0x33); // data read, vram++
    REQUIRE(ppu.getVram() == 0x0fff + 64);
}
