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

    REQUIRE(cpu.GetProgramCounter() == 0x0600);
    REQUIRE(cpu.GetCurrentOpCode() == 0xa9);
}

TEST_CASE("CPU - Successfully runs kThreePixels example")
{
    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, kThreePixels, 15);
    memory.WriteMemory(kReset, (Word)0x0600);
    CPU cpu(&memory);
    cpu.Reset();

    OpCodesTable opcodes;
    while(cpu.GetProgramCounter() < 0x0600 + 15)
    {
        auto opcode = cpu.GetCurrentOpCode();
        cpu.AdvanceProgramCounter();
        opcodes.RunOpCode(&cpu, opcode);
    }

    REQUIRE(memory.ReadByte(0x0200) == 0x01);
    REQUIRE(memory.ReadByte(0x0201) == 0x05);
    REQUIRE(memory.ReadByte(0x0202) == 0x08);
}

// test for setting the status register flags
TEST_CASE("CPU - Succesfully sets appropriate flag on status register")
{
    RawMemoryAccessor memory;
    Registers registers;
    CPU cpu(registers, &memory);
    // set 7th bit (Negative flag)
    cpu.SetStatusRegisterFlag(0b00000001);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001 );

    // set 1st bit (Carry flag)
    cpu.SetStatusRegisterFlag(0b10000000);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100001 );

    // set already set bit
    cpu.SetStatusRegisterFlag(0b10000000);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100001 );
}

TEST_CASE("CPU - Succesfully clears appropriate flag on status register")
{
    RawMemoryAccessor memory;
    Registers registers;
    registers.sr.data = 0b10000101; // test data
    CPU cpu(registers, &memory);
    // clear 7th bit (Negative flag)
    cpu.ClearStatusRegisterFlag(0b10000000);
    REQUIRE(cpu.GetStatusRegister().data == 0b00000101 );

    // clear 1st bit (Carry flag)
    cpu.ClearStatusRegisterFlag(0b00000001);
    REQUIRE(cpu.GetStatusRegister().data == 0b00000100 );

    // clear already cleared bit does nothing
    cpu.ClearStatusRegisterFlag(0b00000001);
    REQUIRE(cpu.GetStatusRegister().data == 0b00000100 );
}

TEST_CASE("CPU - SetStatusRegister Succesfully updates status register")
{
    RawMemoryAccessor memory;
    Registers registers;
    Byte new_sr_data = 0b01010101; // test data
    CPU cpu(registers, &memory);

    cpu.SetStatusRegister(new_sr_data);
    REQUIRE(cpu.GetStatusRegister().data == new_sr_data );
}