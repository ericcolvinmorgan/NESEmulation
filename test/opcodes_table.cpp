#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/constants.h"
#include "../include/emulator/cpu.h"
#include "../include/emulator/raw_memory_accessor.h"
#include "../include/emulator/opcodes_table.h"

TEST_CASE("OpCodes Table - Addressing Mode - None")
{
    RawMemoryAccessor memory;
    CPU cpu(&memory);
    OpCodesTable opcodes;

    auto address = opcodes.AddressingModeNone(&cpu);
    REQUIRE(address == 0);
    REQUIRE(cpu.GetCycleCount() == 0);
}

TEST_CASE("OpCodes Table - Addressing Mode - Implied")
{
    RawMemoryAccessor memory;
    CPU cpu(&memory);
    OpCodesTable opcodes;

    auto address = opcodes.AddressingModeImplied(&cpu);
    REQUIRE(address == 0);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Addressing Mode - Accumulator")
{
    Registers registers{.a = 0xAF};
    RawMemoryAccessor memory;
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address = opcodes.AddressingModeAccumulator(&cpu);
    REQUIRE(address == 0x00AF);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Addressing Mode - Immediate")
{
    Registers registers{.pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Byte)0xAF);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address = opcodes.AddressingModeImmediate(&cpu);
    REQUIRE(address == 0x00AF);
    REQUIRE(cpu.GetCycleCount() == 2);
}