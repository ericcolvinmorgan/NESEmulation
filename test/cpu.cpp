#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/constants.h"
#include "../include/emulator/cpu.h"
#include "../include/emulator/raw_memory_accessor.h"
#include "../include/emulator/opcodes_table.h"

// Below test cases are sourced from discussion at https://skilldrick.github.io/easy6502/
// Load Three Pixels 
const Byte kThreePixels[] = {0xa9, 0x01, 0x8d, 0x00, 0x02, 0xa9, 0x05, 0x8d, 0x01, 0x02, 0xa9, 0x08, 0x8d, 0x02, 0x02};

TEST_CASE("CPU - Successfully loads first OP")
{
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, kThreePixels, 15);
    memory.WriteMemory(kReset, (Word)0x0600);
    CPU cpu(&memory);
    cpu.Reset();
    // OpCodesTable table;
    // for (int i = 0; i <= 0xFF; i++)
    //     cpu.advanceProgramCounter();

    REQUIRE(cpu.GetProgramCounter() == 0x0600);
    REQUIRE(cpu.GetCurrentOpCode() == 0xa9);
}