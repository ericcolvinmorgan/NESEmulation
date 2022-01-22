#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/constants.h"
#include "../include/emulator/cpu.h"
#include "../include/emulator/raw_memory_accessor.h"
#include "../include/emulator/opcodes_table.h"

TEST_CASE("OpCodes Table - Ops - STA - Absolute - Store Accumulator in Memory")
{
    Byte test_case[] = {0x8d, 0x00, 0x02};
    Registers registers{.a = 0xAF, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x8d);
    REQUIRE(memory.ReadByte(0x0200) == 0xAF);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - LDA - Immediate - Load Accumulator with Memory")
{
    Byte test_case[] = {0xa9, 0xAF};
    Registers registers{.a = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xa9);
    REQUIRE(cpu.GetAccumulator() == 0xAF);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - LDA - Zero Page - Load Accumulator with Memory")
{
    Byte test_case[] = {0xA5, 0xA6};
    Registers registers{.a = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x00A6, (Byte)0xA7);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xa5);
    REQUIRE(cpu.GetAccumulator() == 0xA7);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - LDA - Zero Page X - Load Accumulator with Memory")
{
    Byte test_case[] = {0xB5, 0xA6};
    Registers registers{.a = 0x00, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x00A8, (Byte)0xA7);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xB5);
    REQUIRE(cpu.GetAccumulator() == 0xA7);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - LDA - Absolute - Load Accumulator with Memory")
{
    Byte test_case[] = {0xAD, 0x34, 0x12};
    Registers registers{.a = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1234, (Byte)0xA0);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xAD);
    REQUIRE(cpu.GetAccumulator() == 0xA0);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - LDA - Absolute X - Load Accumulator with Memory - No Cross Page")
{
    Byte test_case[] = {0xBD, 0x34, 0x12};
    Registers registers{.a = 0x00, .x = 0x01, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1235, (Byte)0xA1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xBD);
    REQUIRE(cpu.GetAccumulator() == 0xA1);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - LDA - Absolute X - Load Accumulator with Memory - Cross Page")
{
    Byte test_case[] = {0xBD, 0x34, 0x12};
    Registers registers{.a = 0x00, .x = 0xFF, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1333, (Byte)0xA2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xBD);
    REQUIRE(cpu.GetAccumulator() == 0xA2);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - LDA - Absolute Y - Load Accumulator with Memory - No Cross Page")
{
    Byte test_case[] = {0xB9, 0x34, 0x12};
    Registers registers{.a = 0x00, .y = 0x01, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1235, (Byte)0xA1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xB9);
    REQUIRE(cpu.GetAccumulator() == 0xA1);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - LDA - Absolute Y - Load Accumulator with Memory - Cross Page")
{
    Byte test_case[] = {0xB9, 0x34, 0x12};
    Registers registers{.a = 0x00, .y = 0xFF, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1333, (Byte)0xA2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xB9);
    REQUIRE(cpu.GetAccumulator() == 0xA2);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - BRK - Implied - Break via interrupt")
{
    RawMemoryAccessor memory;
    Registers registers;

    // create empty stack
    registers.sp = 0xff;

    // set pc to test address
    registers.pc = 0x8020;

    CPU cpu(registers, &memory);

    // put test bytes in memory
    cpu.WriteMemory((Word)0xFFFE, (Word)0x0055);
    cpu.WriteMemory((Word)0xFFFF, (Word)0x00EE);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x00);

    // check stack data
    REQUIRE(cpu.GetMemoryByte(0x1FF) == 0x80);
    REQUIRE(cpu.GetMemoryByte(0x1FE) == 0x20);
    REQUIRE(cpu.GetMemoryByte(0x1FD) == cpu.GetStatusRegister().data);

    // sp decremented 3x
    REQUIRE(cpu.GetStackPointer() == 0xff - 3);

    // pcl set to 0xFFFE, pch set to 0xFFFF
    REQUIRE(cpu.GetProgramCounter() == 0xEE55);

    // cpu cycle count should increase by 7
    REQUIRE(cpu.GetCycleCount() == 7);
}
