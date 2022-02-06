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

// TEST_CASE("PPU - Write to PPUADDR with mirroring down if address is over 3fff")
// {
//     NESPPUMemoryAccessor *memory;
//     PPU ppu(memory);
//     ppu.WriteToAddrReg(0x4f);
//     ppu.WriteToAddrReg(0xff);

//     REQUIRE(ppu.getVram() == 0x0fff);
// }

// TEST_CASE("PPU - Write to PPUADDR, carry over vram until both writes completed")
// {
//     NESPPUMemoryAccessor *memory;
//     PPU ppu(memory);
//     ppu.WriteToAddrReg(0x4f);
//     ppu.WriteToAddrReg(0xff);

//     REQUIRE(ppu.getVram() == 0x0fff);

//     ppu.WriteToAddrReg(0x12);

//     REQUIRE(ppu.getVram() == 0x0fff); // vram should not be updated yet

//     ppu.WriteToAddrReg(0x34);

//     REQUIRE(ppu.getVram() == 0x1234); // vram should now be oupdated
// }

// TEST_CASE("PPU - 2 cycles of Write to PPUADDR and Read from PPUDATA")
// {
//     NESPPUMemoryAccessor memory;
//     Byte ctrl_flags = 0;
//     memory.WriteMemory((Word)0x0fff, (Byte)0xab);
//     memory.WriteMemory((Word)0x1234, (Byte)0xcd);
//     PPU ppu(&memory, ctrl_flags);

//     // 2 writes to ppuaddr to load vram
//     ppu.WriteToAddrReg(0x4f);
//     ppu.WriteToAddrReg(0xff);
//     REQUIRE(ppu.getVram() == 0x0fff);

//     // 2 reads from ppudata, first is discarded
//     ppu.ReadFromDataReg(); // dummy read
//     // vram gets incremented every read/write to ppudata
//     REQUIRE(ppu.getVram() == 0x0fff + 1);
//     // actual data read
//     REQUIRE(ppu.ReadFromDataReg() == 0xab);

//     // cpu writes to ppuaddr again
//     ppu.WriteToAddrReg(0x12);
//     ppu.WriteToAddrReg(0x34);

//     REQUIRE(ppu.getVram() == 0x1234);
//     ppu.ReadFromDataReg(); // dummy read increments vram
//     REQUIRE(ppu.getVram() == 0x1234 + 1);
//     REQUIRE(ppu.ReadFromDataReg() == 0xcd);
// }