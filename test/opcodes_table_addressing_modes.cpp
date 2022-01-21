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

    auto address_value = opcodes.AddressingModeNone(&cpu);
    REQUIRE(address_value.value == 0);
    REQUIRE(cpu.GetCycleCount() == 0);
}

TEST_CASE("OpCodes Table - Addressing Mode - Implied")
{
    RawMemoryAccessor memory;
    CPU cpu(&memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeImplied(&cpu);
    REQUIRE(address_value.value == 0);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Addressing Mode - Accumulator")
{
    Registers registers{.a = 0xAF};
    RawMemoryAccessor memory;
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeAccumulator(&cpu);
    REQUIRE(address_value.value == 0x00AF);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Addressing Mode - Immediate")
{
    Registers registers{.pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Byte)0xAF);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeImmediate(&cpu);
    REQUIRE(address_value.value == 0x00AF);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Addressing Mode - Absolute")
{
    Registers registers{.pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Word)0xFFF0);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeAbsolute(&cpu);
    REQUIRE(address_value.value == 0xFFF0);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Addressing Mode - Absolute X - No Wrap, No Page Crossing")
{
    Registers registers{.x = 0x05, .pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Word)0xFFF0);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeAbsoluteX(&cpu);
    REQUIRE(address_value.value == 0xFFF5);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Addressing Mode - Absolute X - No Wrap, Page Crossing")
{
    Registers registers{.x = 0xFF, .pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Word)0x09FA);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeAbsoluteX(&cpu);
    REQUIRE(address_value.value == 0x0AF9);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Addressing Mode - Absolute X - With Wrap")
{
    Registers registers{.x = 0xFF, .pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Word)0xFFF0);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeAbsoluteX(&cpu);
    REQUIRE(address_value.value == 0x00EF);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Addressing Mode - Absolute Y - No Wrap, No Page Crossing")
{
    Registers registers{.y = 0x05, .pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Word)0xFFF0);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeAbsoluteY(&cpu);
    REQUIRE(address_value.value == 0xFFF5);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Addressing Mode - Absolute Y - No Wrap, Page Crossing")
{
    Registers registers{.y = 0xFF, .pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Word)0x09FA);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeAbsoluteY(&cpu);
    REQUIRE(address_value.value == 0x0AF9);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Addressing Mode - Absolute Y - With Wrap")
{
    Registers registers{.y = 0xFF, .pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Word)0xFFF0);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeAbsoluteY(&cpu);
    REQUIRE(address_value.value == 0x00EF);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Addressing Mode - Zero Page")
{
    Registers registers{.pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Byte)0xAF);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeZeroPage(&cpu);
    REQUIRE(address_value.value == 0xAF);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Addressing Mode - Zero Page X - Wrap No Carry")
{
    Registers registers{.x = 0x02, .pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Byte)0x10);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeZeroPageX(&cpu);
    REQUIRE(address_value.value == 0x12);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Addressing Mode - Zero Page X - No Wrap")
{
    Registers registers{.x = 0x02, .pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Byte)0xFF);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeZeroPageX(&cpu);
    REQUIRE(address_value.value == 0x01);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Addressing Mode - Zero Page Y - Wrap No Carry")
{
    Registers registers{.y = 0x02, .pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Byte)0x10);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeZeroPageY(&cpu);
    REQUIRE(address_value.value == 0x12);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Addressing Mode - Zero Page Y - No Wrap")
{
    Registers registers{.y = 0x02, .pc = 0x0100};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, (Byte)0xFF);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeZeroPageY(&cpu);
    REQUIRE(address_value.value == 0x01);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Addressing Mode - Relative - Negative Offset")
{
    Registers registers{.pc = 0x0105};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0105, (Byte)0xFA);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeRelative(&cpu);
    REQUIRE(address_value.value == 0x0100);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Addressing Mode - Relative - Negative Offset - Cross Page")
{
    Registers registers{.pc = 0x0105};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0105, (Byte)0xA0);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeRelative(&cpu);
    REQUIRE(address_value.value == 0x00A6);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Addressing Mode - Relative - Positive Offset")
{
    Registers registers{.pc = 0x0105};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0105, (Byte)0x02);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeRelative(&cpu);
    REQUIRE(address_value.value == 0x0108);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Addressing Mode - Relative - Positive Offset - Cross Page")
{
    Registers registers{.pc = 0x01F1};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x01F1, (Byte)0x60);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeRelative(&cpu);
    REQUIRE(address_value.value == 0x0252);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Addressing Mode - Indirect")
{
    Registers registers{.pc = 0x0201};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0201, (Word)0x00F0);
    memory.WriteMemory(0x00F0, (Word)0xCC01);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeAbsoluteIndirect(&cpu);
    REQUIRE(address_value.value == 0xCC01);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Addressing Mode - Indirect X - Wrap No Carry")
{
    Registers registers{.x = 0xFF, .pc = 0x0201};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0201, (Word)0x00F0);
    memory.WriteMemory(0x00EF, (Word)0x0705);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeIndirectX(&cpu);
    REQUIRE(address_value.value == 0x0705);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Addressing Mode - Indirect X - No Carry")
{
    Registers registers{.x = 0x01, .pc = 0x0201};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0201, (Word)0x00F0);
    memory.WriteMemory(0x00F1, (Word)0x0705);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeIndirectX(&cpu);
    REQUIRE(address_value.value == 0x0705);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Addressing Mode - Indirect Y - Cross Page")
{
    Registers registers{.y = 0xFF, .pc = 0x0201};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0201, (Word)0x00F0);
    memory.WriteMemory(0x00F0, (Word)0x0703);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeIndirectY(&cpu);
    REQUIRE(address_value.value == 0x0802);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Addressing Mode - Indirect Y - No Carry")
{
    Registers registers{.y = 0x01, .pc = 0x0201};
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0201, (Word)0x00F0);
    memory.WriteMemory(0x00F0, (Word)0x0703);
    CPU cpu(registers, &memory);
    OpCodesTable opcodes;

    auto address_value = opcodes.AddressingModeIndirectY(&cpu);
    REQUIRE(address_value.value == 0x0704);
    REQUIRE(cpu.GetCycleCount() == 5);
}