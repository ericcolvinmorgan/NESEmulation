#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/cpu.h"
#include "../include/emulator/raw_memory_accessor.h"
#include "../include/emulator/opcodes_table.h"

TEST_CASE("CPU - All OpCodes are not implemented")
{
    RawMemoryAccessor memory;
    CPU cpu(&memory);
    OpCodesTable table;
    for (int i = 0; i <= 0xFF; i++)
        cpu.advanceProgramCounter();

    REQUIRE(cpu.getProgramCounter() == 256);
}