#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include "../include/emulator/constants.h"
#include "../include/emulator/cpu.h"
#include "../include/emulator/raw_memory_accessor.h"
#include "../include/emulator/opcodes_table.h"

struct ImmediateTestCase
{
    Byte test_case[2];
    Registers registers;
    Byte expected_accumulator;
    Byte expected_register;
    uint32_t expected_cycles;
};

TEST_CASE("OpCodes Table - Ops - Throws Exception on Not Implemented")
{
    Byte test_case[] = {0x02, 0xF0};
    Registers registers{.a = 0xAF, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    REQUIRE(opcode == 0x02);

    cpu.AdvanceProgramCounter();
    OpCodesTable opcodes;
    REQUIRE_THROWS(opcodes.RunOpCode(&cpu, opcode));
}

TEST_CASE("OpCodes Table - Ops - STA - Zero Page - Store Accumulator in Memory")
{
    Byte test_case[] = {0x85, 0xF0};
    Registers registers{.a = 0xAF, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x85);
    REQUIRE(memory.ReadByte(0x00F0) == 0xAF);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - STA - Zero Page X - Store Accumulator in Memory")
{
    Byte test_case[] = {0x95, 0xF0};
    Registers registers{.a = 0xAF, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x95);
    REQUIRE(memory.ReadByte(0x00F2) == 0xAF);
    REQUIRE(cpu.GetCycleCount() == 4);
}

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

TEST_CASE("OpCodes Table - Ops - STA - Absolute X - Store Accumulator in Memory")
{
    Byte test_case[] = {0x9d, 0x00, 0x02};
    Registers registers{.a = 0xAF, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x9d);
    REQUIRE(memory.ReadByte(0x0202) == 0xAF);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - STA - Absolute Y - Store Accumulator in Memory")
{
    Byte test_case[] = {0x99, 0x00, 0x02};
    Registers registers{.a = 0xAF, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x99);
    REQUIRE(memory.ReadByte(0x0202) == 0xAF);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - STA - Indirect X - Store Accumulator in Memory")
{
    Byte test_case[] = {0x81, 0xF0};
    Registers registers{.a = 0xAF, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x00F2, (Word)0x1234);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x81);
    REQUIRE(memory.ReadByte(0x1234) == 0xAF);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - STA - Indirect Y - Store Accumulator in Memory")
{
    Byte test_case[] = {0x91, 0xF0};
    Registers registers{.a = 0xAF, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x00F0, (Word)0x1234);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x91);
    REQUIRE(memory.ReadByte(0x1236) == 0xAF);
    REQUIRE(cpu.GetCycleCount() == 6);
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
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
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
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
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
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
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
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
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
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
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
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
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
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
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
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - LDA - Indirect Y - Load Accumulator with Memory - Overflow")
{
    Byte test_case[] = {0xb1, 0x97};
    Registers registers{.a = 0x00, .y = 0x34, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x97, (Byte)0xff);
    memory.WriteMemory(0x98, (Byte)0xff);
    memory.WriteMemory(0x33, (Byte)0xa3);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xb1);
    REQUIRE(cpu.GetAccumulator() == 0xa3);
    REQUIRE(cpu.GetCycleCount() == 6);
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
    cpu.WriteMemory((Word)0xFFFE, (Byte)0x55);
    cpu.WriteMemory((Word)0xFFFF, (Byte)0xEE);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x00);

    StatusRegister copied_sr = cpu.GetStatusRegister();
    copied_sr.flags.b = 1;

    // check stack data
    REQUIRE(cpu.GetMemoryByte(0x1FF) == 0x80);
    REQUIRE(cpu.GetMemoryByte(0x1FE) == 0x20);
    REQUIRE(cpu.GetMemoryByte(0x1FD) == copied_sr.data);

    // sp decremented 3x
    REQUIRE(cpu.GetStackPointer() == 0xff - 3);

    // pcl set to 0xFFFE, pch set to 0xFFFF
    REQUIRE(cpu.GetProgramCounter() == 0xEE55);

    REQUIRE(cpu.GetStatusRegister().flags.b == 0);

    // cpu cycle count should increase by 7
    REQUIRE(cpu.GetCycleCount() == 7);
}



TEST_CASE("OpCodes Table - Ops - PHA - Implied - Push accumulator on stack")
{
    RawMemoryAccessor memory;
    Registers registers{.a = 0x4D, .sp = 0xFF}; // test data
    CPU cpu(registers, &memory);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x48);

    // check top of stack
    REQUIRE(cpu.GetMemoryByte(0x100 + (cpu.GetStackPointer() + 1)) == cpu.GetAccumulator());

    // sp decremented once
    REQUIRE(cpu.GetStackPointer() == 0xFF - 1);

    // cpu cycle count should increase by 3
    REQUIRE(cpu.GetCycleCount() == 3);
}

// TEST_CASE("OpCodes Table - Ops - PLA - Implied - Store top of stack in accumulator")
// {
//     RawMemoryAccessor memory;
//     Registers registers{.a = 0x4D, .sp = 0xFE}; // test data
//     CPU cpu(registers, &memory);
//     Byte new_accumulator = 0x3D;
//     // write test data to top of stack
//     cpu.WriteMemory(0x100 + (cpu.GetStackPointer() + 1), (Byte)new_accumulator);

//     OpCodesTable opcodes;
//     opcodes.RunOpCode(&cpu, 0x68);

//     // check accumulator for new val
//     REQUIRE(cpu.GetAccumulator() == new_accumulator);

//     // sp incremented once
//     REQUIRE(cpu.GetStackPointer() == 0xFE + 1);

//     // cpu cycle count should increase by 4
//     REQUIRE(cpu.GetCycleCount() == 4);
// }

TEST_CASE("OpCodes Table - Ops - PLA - Implied - Negative flag updates")
{
    RawMemoryAccessor memory;
    Registers registers {.a = 0x4D, .sp = 0xFE}; // test data
    CPU cpu(registers, &memory);
    Byte new_accumulator = 0xFF;
    // write test data to top of stack
    cpu.WriteMemory(0x100 + (cpu.GetStackPointer() + 1), (Byte)new_accumulator);
    
    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x68);
    
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);

}

TEST_CASE("OpCodes Table - Ops - PLA - Implied - Zero flag updates")
{
    RawMemoryAccessor memory;
    Registers registers {.a = 0x4D, .sp = 0xFE}; // test data
    CPU cpu(registers, &memory);
    Byte new_accumulator = 0x00;
    // write test data to top of stack
    cpu.WriteMemory(0x100 + (cpu.GetStackPointer() + 1), (Byte)new_accumulator);
    
    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x68);

    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
}

TEST_CASE("OpCodes Table - Ops - RTI - Implied - Return from Interrupt ")
{
    RawMemoryAccessor memory;
    Byte top_of_stack = 0xFF;
    Registers registers{.sp = (Byte)(top_of_stack - 3)};
    CPU cpu(registers, &memory);
    // test data
    Byte new_sr = 0b10101011;
    Byte pc_l = 0x50;
    Byte pc_h = 0x80;

    // write test data
    memory.WriteMemory(0x100 + top_of_stack, (Byte)pc_h);
    memory.WriteMemory(0x100 + (top_of_stack - 1), (Byte)pc_l);
    memory.WriteMemory(0x100 + (top_of_stack - 2), (Byte)new_sr);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x40);

    // check updated status register
    REQUIRE(cpu.GetStatusRegister().data == new_sr);

    // sp incremented 3 times
    REQUIRE(cpu.GetStackPointer() == top_of_stack);

    // pc = 0x5080
    REQUIRE(cpu.GetProgramCounter() == 0x8050);

    // cpu cycle count should increase by 6
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - RTS - Implied - Return from subroutine ")
{
    RawMemoryAccessor memory;
    Byte top_of_stack = 0xFF;
    Registers registers{.sp = (Byte)(top_of_stack - 2)};
    CPU cpu(registers, &memory);
    // test data
    Byte pc_l = 0x02;
    Byte pc_h = 0x01;

    // write test data
    memory.WriteMemory(0x100 + top_of_stack, (Byte)pc_h);
    memory.WriteMemory(0x100 + (top_of_stack - 1), (Byte)pc_l);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x60);

    // sp incremented 2 times
    REQUIRE(cpu.GetStackPointer() == top_of_stack);

    // pc set to $0102 then incremented, so should equal $0103
    REQUIRE(cpu.GetProgramCounter() == 0x0103);

    // cpu cycle count should increase by 6
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - JSR - Absolute - Jump to subroutine ")
{
    RawMemoryAccessor memory;
    Byte top_of_stack = 0xFF;
    Registers registers{.sp = top_of_stack, .pc = 0x0101};
    CPU cpu(registers, &memory);
    // test data
    Byte hi = 0x12;
    Byte lo = 0x34;

    // write test data
    cpu.WriteMemory(0x0101, (Byte)lo);
    cpu.WriteMemory(0x0102, (Byte)hi);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x20);

    // check stack
    REQUIRE(cpu.GetStackPointer() == top_of_stack - 2);
    REQUIRE(cpu.GetMemoryByte(0x100 + top_of_stack) == 0x01);
    REQUIRE(cpu.GetMemoryByte(0x100 + (top_of_stack - 1)) == 0x02);

    // pc set to $1234
    REQUIRE(cpu.GetProgramCounter() == 0x1234);

    // cpu cycle count should increase by 6
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - JSR and RTS are synchronized ")
{
    /*
        PC $0100 JMP instruction read
        PC $0101 fetch low address (0x34)
        PC $0102 fetch high address (0x12)
        $0102 pushed to stack
        -jump to subroutine at 0x1234
        -subroutine completes
        pull $0102 off stack and put in PC, RTS increments PC
        PC $0103 reads next instruction to execute
    */
    RawMemoryAccessor memory;
    Byte top_of_stack = 0xFF;
    Registers registers{.sp = top_of_stack, .pc = 0x0101};
    CPU cpu(registers, &memory);
    // test data
    Byte hi = 0x12;
    Byte lo = 0x34;

    // write test data
    cpu.WriteMemory(0x0101, (Byte)lo);
    cpu.WriteMemory(0x0102, (Byte)hi);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x20); // JSR
    opcodes.RunOpCode(&cpu, 0x60); // RTS

    // stack should be empty again
    REQUIRE(cpu.GetStackPointer() == top_of_stack);

    REQUIRE(cpu.GetProgramCounter() == 0x103);
}

TEST_CASE("OpCodes Table - Ops - ORA - Immediate - OR Memory with Accumulator")
{
    Byte test_case[] = {0x09, 0xaa};
    Registers registers{.a = 0x55, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x09);
    REQUIRE(cpu.GetAccumulator() == 0xff);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - ORA - Zero Page - OR Memory with Accumulator")
{
    Byte test_case[] = {0x05, 0x34};
    Registers registers{.a = 0x55, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Byte)0xaa);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x05);
    REQUIRE(cpu.GetAccumulator() == 0xff);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - ORA - Zero Page X - OR Memory with Accumulator")
{
    Byte test_case[] = {0x15, 0x34};
    Registers registers{.a = 0x55, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0036, (Byte)0xaa);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x15);
    REQUIRE(cpu.GetAccumulator() == 0xff);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - ORA - Absolute - OR Memory with Accumulator")
{
    Byte test_case[] = {0x0d, 0x34, 0x12};
    Registers registers{.a = 0x55, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1234, (Byte)0xaa);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x0d);
    REQUIRE(cpu.GetAccumulator() == 0xff);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - ORA - Absolute X - OR Memory with Accumulator")
{
    Byte test_case[] = {0x1d, 0x34, 0x12};
    Registers registers{.a = 0x55, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1236, (Byte)0xaa);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x1d);
    REQUIRE(cpu.GetAccumulator() == 0xff);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - ORA - Absolute Y - OR Memory with Accumulator")
{
    Byte test_case[] = {0x19, 0x34, 0x12};
    Registers registers{.a = 0x55, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1236, (Byte)0xaa);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x19);
    REQUIRE(cpu.GetAccumulator() == 0xff);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - ORA - Indirect X - OR Memory with Accumulator")
{
    Byte test_case[] = {0x01, 0x34};
    Registers registers{.a = 0x55, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0036, (Word)0x1236);
    memory.WriteMemory(0x1236, (Byte)0xaa);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x01);
    REQUIRE(cpu.GetAccumulator() == 0xff);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - ORA - Indirect Y - OR Memory with Accumulator")
{
    Byte test_case[] = {0x11, 0x34};
    Registers registers{.a = 0x55, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Word)0x1234);
    memory.WriteMemory(0x1236, (Byte)0xaa);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x11);
    REQUIRE(cpu.GetAccumulator() == 0xff);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - LDX - Immediate - Load Index Register X From Memory")
{
    Byte test_case[] = {0xa2, 0xAF};
    Registers registers{.x = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xa2);
    REQUIRE(cpu.GetXIndex() == 0xAF);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - LDX - Zero Page - Load Index Register X From Memory")
{
    Byte test_case[] = {0xa6, 0xAF};
    Registers registers{.x = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x00AF, (Byte)0xBC);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xa6);
    REQUIRE(cpu.GetXIndex() == 0xBC);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - LDX - Absolute - Load Index Register X From Memory")
{
    Byte test_case[] = {0xae, 0xAF, 0x08};
    Registers registers{.x = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x08AF, (Byte)0xA0);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xae);
    REQUIRE(cpu.GetXIndex() == 0xA0);
    REQUIRE(cpu.GetCycleCount() == 4);
}


TEST_CASE("OpCodes Table - Ops - LDX - Zero Page Y - Load Index Register X From Memory")
{
    Byte test_case[] = {0xb6, 0xAF};
    Registers registers{.x = 0x00, .y = 0x10, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x00BF, (Byte) 0xA0);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xb6);
    REQUIRE(cpu.GetXIndex() == 0xA0);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - LDX - Absolute Y - Load Index Register X From Memory")
{
    Byte test_case[] = {0xbe, 0xAF, 0x12};
    Registers registers{.x = 0x00, .y = 0x21,.pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x12D0, (Byte)0x73);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xbe);
    REQUIRE(cpu.GetXIndex() == 0x73);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - LDX - Zero flag sets when load zero")
{
    Byte test_case[] = {0xa2, 0x00};
    Registers registers{.x = 0xA0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);
    const auto statusRegister = cpu.GetStatusRegister();
    REQUIRE(statusRegister.flags.z == 1);
}

TEST_CASE("OpCodes Table - Ops - LDX - Zero flag clears when load non-zero")
{
    Byte test_case[] = {0xa2, 0x00, 0xa2, 0x04};
    Registers registers{.x = 0xA0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 4);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);
    auto statusRegister = cpu.GetStatusRegister();
    REQUIRE(statusRegister.flags.z == 1);

    opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    opcodes.RunOpCode(&cpu, opcode);
    statusRegister = cpu.GetStatusRegister();
    REQUIRE(statusRegister.flags.z == 0);
}

TEST_CASE("OpCodes Table - Ops - LDX - Negative flag sets when load negative")
{
    Byte test_case[] = {0xa2, 0xA0};
    Registers registers{.x = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);
    const auto statusRegister = cpu.GetStatusRegister();
    REQUIRE(statusRegister.flags.n == 1);
}

TEST_CASE("OpCodes Table - Ops - LDX - Negative flag clears when load non-negative")
{
    Byte test_case[] = {0xa2, 0xA0, 0xa2, 0x00};
    Registers registers{.x = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 4);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);
    auto statusRegister = cpu.GetStatusRegister();
    REQUIRE(statusRegister.flags.n == 1);

    opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    opcodes.RunOpCode(&cpu, opcode);
    statusRegister = cpu.GetStatusRegister();
    REQUIRE(statusRegister.flags.n == 0);
}

TEST_CASE("OpCodes Table - Ops - STX - Zero Page - Store Index Register X In Memory")
{
    Byte test_case[] = {0x86, 0x7C};
    Registers registers{.x = 0x73, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x86);
    REQUIRE(memory.ReadByte(0x007C) == 0x73);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - STX - Absolute - Store Index Register X In Memory")
{
    Byte test_case[] = {0x8e, 0xAF, 0x08};
    Registers registers{.x = 0x23, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x8e);
    REQUIRE(memory.ReadByte(0x08AF) == 0x23);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - STX - Zero Page Y - Store Index Register X In Memory")
{
    Byte test_case[] = {0x96, 0xAF};
    Registers registers{.x = 0x45, .y = 0x10, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x96);
    REQUIRE(memory.ReadByte(0x00BF) == 0x45);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - SED - Set decimal mode flag ")
{
    
    Byte test_case[] = {0xF8, 0x12};
    Registers registers{ .pc = 0x0100 };

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    REQUIRE(cpu.GetStatusRegister().flags.d == 0);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(cpu.GetStatusRegister().flags.d == 1);
}

TEST_CASE("OpCodes Table - Ops - CLD - Clear decimal mode flag ")
{
    
    Byte test_case[] = {0xD8, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.d = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    REQUIRE(cpu.GetStatusRegister().flags.d == 1);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(cpu.GetStatusRegister().flags.d == 0);
}

TEST_CASE("OpCodes Table - Ops - SEI - Set interrupt disable flag ")
{
    
    Byte test_case[] = {0x78, 0x12};
    Registers registers{ .pc = 0x0100 };

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    REQUIRE(cpu.GetStatusRegister().flags.i == 0);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(cpu.GetStatusRegister().flags.i == 1);
}

TEST_CASE("OpCodes Table - Ops - CLI - Clear interrupt disable flag ")
{
    Byte test_case[] = {0x58, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.i = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    REQUIRE(cpu.GetStatusRegister().flags.i == 1);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(cpu.GetStatusRegister().flags.i == 0);
}

TEST_CASE("OpCodes Table - Ops - SEC - Set carry flag ")
{
    Byte test_case[] = {0x38, 0x12};
    Registers registers{ .pc = 0x0100 };

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    REQUIRE(cpu.GetStatusRegister().flags.c == 0);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
}

TEST_CASE("OpCodes Table - Ops - CLC - Clear carry flag ")
{
    Byte test_case[] = {0x18, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.c = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    REQUIRE(cpu.GetStatusRegister().flags.c == 1);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(cpu.GetStatusRegister().flags.c == 0);
}

TEST_CASE("OpCodes Table - Ops - CLV - Clear overflow flag ")
{
    Byte test_case[] = {0xb8, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.o = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    REQUIRE(cpu.GetStatusRegister().flags.o == 1);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(cpu.GetStatusRegister().flags.o == 0);
}

TEST_CASE("OpCodes Table - Ops - AND - Immediate - AND Memory with Accumulator")
{
    Byte test_case[] = {0x29, 0x91};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x29);
    REQUIRE(cpu.GetAccumulator() == 0x81);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - AND - Zero Page - AND Memory with Accumulator")
{
    Byte test_case[] = {0x25, 0x34};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x25);
    REQUIRE(cpu.GetAccumulator() == 0x81);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - AND - Zero Page X - AND Memory with Accumulator")
{
    Byte test_case[] = {0x35, 0x34};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0036, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x35);
    REQUIRE(cpu.GetAccumulator() == 0x81);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - AND - Absolute - AND Memory with Accumulator")
{
    Byte test_case[] = {0x2d, 0x34, 0x12};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1234, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x2d);
    REQUIRE(cpu.GetAccumulator() == 0x81);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - AND - Absolute X - AND Memory with Accumulator")
{
    Byte test_case[] = {0x3d, 0x34, 0x12};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x3d);
    REQUIRE(cpu.GetAccumulator() == 0x81);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - AND - Absolute Y - AND Memory with Accumulator")
{
    Byte test_case[] = {0x39, 0x34, 0x12};
    Registers registers{.a = 0xaf, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x39);
    REQUIRE(cpu.GetAccumulator() == 0x81);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - AND - Indirect X - AND Memory with Accumulator")
{
    Byte test_case[] = {0x21, 0x34};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0036, (Word)0x1236);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x21);
    REQUIRE(cpu.GetAccumulator() == 0x81);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - AND - Indirect Y - AND Memory with Accumulator")
{
    Byte test_case[] = {0x31, 0x34};
    Registers registers{.a = 0xaf, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Word)0x1234);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x31);
    REQUIRE(cpu.GetAccumulator() == 0x81);
    REQUIRE(cpu.GetStatusRegister().data == 0b10100000);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - EOR - Immediate - EOR Memory with Accumulator")
{
    Byte test_case[] = {0x49, 0x91};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x49);
    REQUIRE(cpu.GetAccumulator() == 0x3e);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100000);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - EOR - Zero Page - EOR Memory with Accumulator")
{
    Byte test_case[] = {0x45, 0x34};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x45);
    REQUIRE(cpu.GetAccumulator() == 0x3e);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100000);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - EOR - Zero Page X - EOR Memory with Accumulator")
{
    Byte test_case[] = {0x55, 0x34};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0036, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x55);
    REQUIRE(cpu.GetAccumulator() == 0x3e);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100000);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - EOR - Absolute - EOR Memory with Accumulator")
{
    Byte test_case[] = {0x4d, 0x34, 0x12};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1234, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x4d);
    REQUIRE(cpu.GetAccumulator() == 0x3e);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100000);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - EOR - Absolute X - EOR Memory with Accumulator")
{
    Byte test_case[] = {0x5d, 0x34, 0x12};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x5d);
    REQUIRE(cpu.GetAccumulator() == 0x3e);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100000);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - EOR - Absolute Y - EOR Memory with Accumulator")
{
    Byte test_case[] = {0x59, 0x34, 0x12};
    Registers registers{.a = 0xaf, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x59);
    REQUIRE(cpu.GetAccumulator() == 0x3e);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100000);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - EOR - Indirect X - EOR Memory with Accumulator")
{
    Byte test_case[] = {0x41, 0x34};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0036, (Word)0x1236);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x41);
    REQUIRE(cpu.GetAccumulator() == 0x3e);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100000);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - EOR - Indirect Y - EOR Memory with Accumulator")
{
    Byte test_case[] = {0x51, 0x34};
    Registers registers{.a = 0xaf, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Word)0x1234);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x51);
    REQUIRE(cpu.GetAccumulator() == 0x3e);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100000);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - ADC - Immediate - Add Memory to Accumulator with Carry")
{
    Byte test_case[] = {0x69, 0x91};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x69);
    REQUIRE(cpu.GetAccumulator() == 0x40);
    REQUIRE(cpu.GetStatusRegister().data == 0b01100001);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - ADC - Immediate - Various Test Cases")
{
    // Examples per http://www.6502.org/tutorials/vflag.html
    auto test_data = GENERATE(table<struct ImmediateTestCase>({{{.test_case = {0x69, 0x01}, .registers = {.a = 0x01, .sr = {.data = 0b00100000}, .pc = 0x0600}, 0x02, 0b00100000, 2}},
                                                               {{.test_case = {0x69, 0xff}, .registers = {.a = 0x01, .sr = {.data = 0b00100000}, .pc = 0x0600}, 0x00, 0b00100011, 2}},
                                                               {{.test_case = {0x69, 0x01}, .registers = {.a = 0x7f, .sr = {.data = 0b00100000}, .pc = 0x0600}, 0x80, 0b11100000, 2}},
                                                               {{.test_case = {0x69, 0xff}, .registers = {.a = 0x80, .sr = {.data = 0b00100000}, .pc = 0x0600}, 0x7f, 0b01100001, 2}}}));

    auto test_case = std::get<0>(test_data);

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case.test_case, 2);

    CPU cpu(test_case.registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(cpu.GetAccumulator() == test_case.expected_accumulator);
    REQUIRE(cpu.GetStatusRegister().data == test_case.expected_register);
    REQUIRE(cpu.GetCycleCount() == test_case.expected_cycles);
}

TEST_CASE("OpCodes Table - Ops - ADC - Zero Page - Add Memory to Accumulator with Carry")
{
    Byte test_case[] = {0x65, 0x34};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x65);
    REQUIRE(cpu.GetAccumulator() == 0x40);
    REQUIRE(cpu.GetStatusRegister().data == 0b01100001);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - ADC - Zero Page X - Add Memory to Accumulator with Carry")
{
    Byte test_case[] = {0x75, 0x34};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0036, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x75);
    REQUIRE(cpu.GetAccumulator() == 0x40);
    REQUIRE(cpu.GetStatusRegister().data == 0b01100001);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - ADC - Absolute - Add Memory to Accumulator with Carry")
{
    Byte test_case[] = {0x6d, 0x34, 0x12};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1234, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x6d);
    REQUIRE(cpu.GetAccumulator() == 0x40);
    REQUIRE(cpu.GetStatusRegister().data == 0b01100001);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - ADC - Absolute X - Add Memory to Accumulator with Carry")
{
    Byte test_case[] = {0x7d, 0x34, 0x12};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x7d);
    REQUIRE(cpu.GetAccumulator() == 0x40);
    REQUIRE(cpu.GetStatusRegister().data == 0b01100001);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - ADC - Absolute Y - Add Memory to Accumulator with Carry")
{
    Byte test_case[] = {0x79, 0x34, 0x12};
    Registers registers{.a = 0xaf, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x79);
    REQUIRE(cpu.GetAccumulator() == 0x40);
    REQUIRE(cpu.GetStatusRegister().data == 0b01100001);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - ADC - Indirect X - Add Memory to Accumulator with Carry")
{
    Byte test_case[] = {0x61, 0x34};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0036, (Word)0x1236);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x61);
    REQUIRE(cpu.GetAccumulator() == 0x40);
    REQUIRE(cpu.GetStatusRegister().data == 0b01100001);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - ADC - Indirect Y - Add Memory to Accumulator with Carry")
{
    Byte test_case[] = {0x71, 0x34};
    Registers registers{.a = 0x02, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Word)0x1234);
    memory.WriteMemory(0x1236, (Byte)0x20);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x71);
    REQUIRE(cpu.GetAccumulator() == 0x22);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100000);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - CMP - Immediate - Compare Memory with Accumulator")
{
    Byte test_case[] = {0xc9, 0x91};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xc9);
    REQUIRE(cpu.GetAccumulator() == 0xaf);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - CMP - Immediate - Various Test Cases")
{
    // Examples per http://www.6502.org/tutorials/vflag.html
    auto test_data = GENERATE(table<struct ImmediateTestCase>({{{.test_case = {0xc9, 0x01}, .registers = {.a = 0x00, .sr = {.data = 0b00100000}, .pc = 0x0600}, 0x00, 0b10100000, 2}},
                                                               {{.test_case = {0xc9, 0x01}, .registers = {.a = 0x80, .sr = {.data = 0b00100000}, .pc = 0x0600}, 0x80, 0b00100001, 2}},
                                                               {{.test_case = {0xc9, 0x70}, .registers = {.a = 0x70, .sr = {.data = 0b00100000}, .pc = 0x0600}, 0x70, 0b00100011, 2}},
                                                               {{.test_case = {0xc9, 0xff}, .registers = {.a = 0x7f, .sr = {.data = 0b00100000}, .pc = 0x0600}, 0x7f, 0b10100000, 2}}}));

    auto test_case = std::get<0>(test_data);

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case.test_case, 2);

    CPU cpu(test_case.registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(cpu.GetAccumulator() == test_case.expected_accumulator);
    REQUIRE(cpu.GetStatusRegister().data == test_case.expected_register);
    REQUIRE(cpu.GetCycleCount() == test_case.expected_cycles);
}

TEST_CASE("OpCodes Table - Ops - CMP - Zero Page - Compare Memory with Accumulator")
{
    Byte test_case[] = {0xc5, 0x34};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xc5);
    REQUIRE(cpu.GetAccumulator() == 0xaf);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - CMP - Zero Page X - Compare Memory with Accumulator")
{
    Byte test_case[] = {0xd5, 0x34};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0036, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xd5);
    REQUIRE(cpu.GetAccumulator() == 0xaf);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - CMP - Absolute - Compare Memory with Accumulator")
{
    Byte test_case[] = {0xcd, 0x34, 0x12};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1234, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xcd);
    REQUIRE(cpu.GetAccumulator() == 0xaf);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - CMP - Absolute X - Compare Memory with Accumulator")
{
    Byte test_case[] = {0xdd, 0x34, 0x12};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xdd);
    REQUIRE(cpu.GetAccumulator() == 0xaf);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - CMP - Absolute Y - Compare Memory with Accumulator")
{
    Byte test_case[] = {0xd9, 0x34, 0x12};
    Registers registers{.a = 0xaf, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xd9);
    REQUIRE(cpu.GetAccumulator() == 0xaf);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - CMP - Indirect X - Compare Memory with Accumulator")
{
    Byte test_case[] = {0xc1, 0x34};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0036, (Word)0x1236);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xc1);
    REQUIRE(cpu.GetAccumulator() == 0xaf);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - CMP - Indirect Y - Compare Memory with Accumulator")
{
    Byte test_case[] = {0xd1, 0x34};
    Registers registers{.a = 0xaf, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Word)0x1234);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xd1);
    REQUIRE(cpu.GetAccumulator() == 0xaf);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - SBC - Immediate - Subtract Memory from Accumulator with Borrow")
{
    Byte test_case[] = {0xe9, 0x91};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xe9);
    REQUIRE(cpu.GetAccumulator() == 0x1d);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - SBC - Immediate - Various Test Cases")
{
    // Examples per http://www.6502.org/tutorials/vflag.html
    auto test_data = GENERATE(table<struct ImmediateTestCase>({{{.test_case = {0xe9, 0x01}, .registers = {.a = 0x00, .sr = {.data = 0b00100001}, .pc = 0x0600}, 0xff, 0b10100000, 2}},
                                                               {{.test_case = {0xe9, 0x01}, .registers = {.a = 0x80, .sr = {.data = 0b00100001}, .pc = 0x0600}, 0x7f, 0b01100001, 2}},
                                                               {{.test_case = {0xe9, 0xff}, .registers = {.a = 0x7f, .sr = {.data = 0b00100001}, .pc = 0x0600}, 0x80, 0b11100000, 2}}}));

    auto test_case = std::get<0>(test_data);

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case.test_case, 2);

    CPU cpu(test_case.registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(cpu.GetAccumulator() == test_case.expected_accumulator);
    REQUIRE(cpu.GetStatusRegister().data == test_case.expected_register);
    REQUIRE(cpu.GetCycleCount() == test_case.expected_cycles);
}

TEST_CASE("OpCodes Table - Ops - SBC - Zero Page - Subtract Memory from Accumulator with Borrow")
{
    Byte test_case[] = {0xe5, 0x34};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xe5);
    REQUIRE(cpu.GetAccumulator() == 0x1d);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - SBC - Zero Page X - Subtract Memory from Accumulator with Borrow")
{
    Byte test_case[] = {0xf5, 0x34};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0036, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xf5);
    REQUIRE(cpu.GetAccumulator() == 0x1d);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - SBC - Absolute - Subtract Memory from Accumulator with Borrow")
{
    Byte test_case[] = {0xed, 0x34, 0x12};
    Registers registers{.a = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1234, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xed);
    REQUIRE(cpu.GetAccumulator() == 0x1d);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - SBC - Absolute X - Subtract Memory from Accumulator with Borrow")
{
    Byte test_case[] = {0xfd, 0x34, 0x12};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xfd);
    REQUIRE(cpu.GetAccumulator() == 0x1d);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - SBC - Absolute Y - Subtract Memory from Accumulator with Borrow")
{
    Byte test_case[] = {0xf9, 0x34, 0x12};
    Registers registers{.a = 0xaf, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xf9);
    REQUIRE(cpu.GetAccumulator() == 0x1d);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - SBC - Indirect X - Subtract Memory from Accumulator with Borrow")
{
    Byte test_case[] = {0xe1, 0x34};
    Registers registers{.a = 0xaf, .x = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0036, (Word)0x1236);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xe1);
    REQUIRE(cpu.GetAccumulator() == 0x1d);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - SBC - Indirect Y - Subtract Memory from Accumulator with Borrow")
{
    Byte test_case[] = {0xf1, 0x34};
    Registers registers{.a = 0xaf, .y = 0x02, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Word)0x1234);
    memory.WriteMemory(0x1236, (Byte)0x91);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xf1);
    REQUIRE(cpu.GetAccumulator() == 0x1d);
    REQUIRE(cpu.GetStatusRegister().data == 0b00100001);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - ASL - Zero Page - Arithmetic Shift Left")
{
    Byte test_case[] = {0x06, 0x34};
    Registers registers{.pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Byte)0b10101011);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x06);
    REQUIRE(memory.ReadByte(0x0034) == 0b01010110);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - ASL - Accumulator - Arithmetic Shift Left")
{
    Registers registers{.a = 0b10000000};

    RawMemoryAccessor memory;

    CPU cpu(registers, &memory);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x0a);

    REQUIRE(cpu.GetAccumulator() == 0x00);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - ASL - Absolute - Arithmetic Shift Left")
{
    Byte test_case[] = {0x0e, 0x34, 0xAF};
    Registers registers{.pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0xAF34, (Byte)0b00001110);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x0e);
    REQUIRE(memory.ReadByte(0xAF34) == 0b00011100);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 0);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - ASL - Zero Page X - Arithmetic Shift Left")
{
    Byte test_case[] = {0x16, 0x34};
    Registers registers{.x = 0x32, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0066, (Byte)0b10101011);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x16);
    REQUIRE(memory.ReadByte(0x0066) == 0b01010110);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - ASL - Absolute X - Arithmetic Shift Left")
{
    Byte test_case[] = {0x1e, 0x34, 0x12};
    Registers registers{.x = 0x01, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1235, (Byte)0b10101011);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x1e);
    REQUIRE(memory.ReadByte(0x1235) == 0b01010110);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 7);
}

TEST_CASE("OpCodes Table - Ops - LSR - Zero Page - Logical Shift Right")
{
    Byte test_case[] = {0x46, 0x34};
    Registers registers{.pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Byte)0b10101011);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x46);
    REQUIRE(memory.ReadByte(0x0034) == 0b01010101);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - LSR - Accumulator - Logical Shift Right")
{
    Registers registers{.a = 0b00000001};

    RawMemoryAccessor memory;

    CPU cpu(registers, &memory);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x4a);

    REQUIRE(cpu.GetAccumulator() == 0x00);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - LSR - Absolute - Logical Shift Right")
{
    Byte test_case[] = {0x4e, 0x34, 0xAF};
    Registers registers{.pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0xAF34, (Byte)0b00001110);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x4e);
    REQUIRE(memory.ReadByte(0xAF34) == 0b00000111);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 0);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - LSR - Zero Page X - Logical Shift Right")
{
    Byte test_case[] = {0x56, 0x34};
    Registers registers{.x = 0x32, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0066, (Byte)0b10101011);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x56);
    REQUIRE(memory.ReadByte(0x0066) == 0b01010101);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - LSR - Absolute X - Logical Shift Right")
{
    Byte test_case[] = {0x5e, 0x34, 0x12};
    Registers registers{.x = 0x01, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1235, (Byte)0b10101011);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x5e);
    REQUIRE(memory.ReadByte(0x1235) == 0b01010101);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 7);
}

TEST_CASE("OpCodes Table - Ops - ROL - Zero Page - Rotate Left")
{
    Byte test_case[] = {0x26, 0x34};
    Registers registers{.pc = 0x0600};
    registers.sr.flags.c = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Byte)0b10101011);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x26);
    REQUIRE(memory.ReadByte(0x0034) == 0b01010111);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - ROL - Accumulator - Rotate Left")
{
    Registers registers{.a = 0b10000000};

    RawMemoryAccessor memory;

    CPU cpu(registers, &memory);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x2a);

    REQUIRE(cpu.GetAccumulator() == 0x00);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - ROL - Absolute - Rotate Left")
{
    Byte test_case[] = {0x2e, 0x34, 0xAF};
    Registers registers{.pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0xAF34, (Byte)0b00001110);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x2e);
    REQUIRE(memory.ReadByte(0xAF34) == 0b00011100);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 0);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - ROL - Zero Page X - Rotate Left")
{
    Byte test_case[] = {0x36, 0x34};
    Registers registers{.x = 0x32, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0066, (Byte)0b11101011);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x36);
    REQUIRE(memory.ReadByte(0x0066) == 0b11010110);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - ROL - Absolute X - Rotate Left")
{
    Byte test_case[] = {0x3e, 0x34, 0x12};
    Registers registers{.x = 0x01, .pc = 0x0600};
    registers.sr.flags.c = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1235, (Byte)0b10101011);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x3e);
    REQUIRE(memory.ReadByte(0x1235) == 0b01010111);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 7);
}

TEST_CASE("OpCodes Table - Ops - ROR - Zero Page - Rotate Right")
{
    Byte test_case[] = {0x66, 0x34};
    Registers registers{.pc = 0x0600};
    registers.sr.flags.c = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0034, (Byte)0b10101011);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x66);
    REQUIRE(memory.ReadByte(0x0034) == 0b11010101);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - ROR - Accumulator - Rotate Right")
{
    Registers registers{.a = 0b00000001};

    RawMemoryAccessor memory;

    CPU cpu(registers, &memory);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x6a);

    REQUIRE(cpu.GetAccumulator() == 0x00);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - ROR - Absolute - Rotate Right")
{
    Byte test_case[] = {0x6e, 0x34, 0xAF};
    Registers registers{.pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0xAF34, (Byte)0b00001110);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x6e);
    REQUIRE(memory.ReadByte(0xAF34) == 0b00000111);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 0);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - ROR - Zero Page X - Rotate Right")
{
    Byte test_case[] = {0x76, 0x34};
    Registers registers{.x = 0x32, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x0066, (Byte)0b11101011);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x76);
    REQUIRE(memory.ReadByte(0x0066) == 0b01110101);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - ROR - Absolute X - Rotate Right")
{
    Byte test_case[] = {0x7e, 0x34, 0x12};
    Registers registers{.x = 0x01, .pc = 0x0600};
    registers.sr.flags.c = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1235, (Byte)0b10101011);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x7e);
    REQUIRE(memory.ReadByte(0x1235) == 0b11010101);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetCycleCount() == 7);
}

TEST_CASE("OpCodes Table - Ops - INY - Implied - Increment Y Index with Wrap Around")
{
    Byte test_case[] = {0xc8};
    Registers registers{.y = 0xff, .pc = 0x0600};
    registers.sr.flags.c = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xc8);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetYIndex() == 0x00);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - INY - Implied - Increment Y Index without Wrap Around")
{
    Byte test_case[] = {0xc8};
    Registers registers{.y = 0xad, .pc = 0x0600};
    registers.sr.flags.c = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xc8);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetYIndex() == 0xae);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - INX - Implied - Increment X Index with Wrap Around")
{
    Byte test_case[] = {0xe8};
    Registers registers{.x = 0xff, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xe8);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetXIndex() == 0x00);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - INX - Implied - Increment X Index without Wrap Around")
{
    Byte test_case[] = {0xe8};
    Registers registers{.x = 0xad, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xe8);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetXIndex() == 0xae);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - DEY - Implied - Decrement Y Index with Wrap Around")
{
    Byte test_case[] = {0x88};
    Registers registers{.y = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x88);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetYIndex() == 0xff);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - DEY - Implied - Decrement Y Index without Wrap Around")
{
    Byte test_case[] = {0x88};
    Registers registers{.y = 0x01, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x88);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetYIndex() == 0x00);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - TAY - Implied - Copy Accumulator to Y Register - zero flag")
{
    Byte test_case[] = {0xa8};
    Registers registers{.a = 0x00, .y = 0x01, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    REQUIRE(cpu.GetStatusRegister().flags.z == 0);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xa8);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetYIndex() == 0x00);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - TAY - Implied - Copy Accumulator to Y Register - negative flag")
{
    Byte test_case[] = {0xa8};
    Registers registers{.a = 0xaf, .y = 0x01, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    REQUIRE(cpu.GetStatusRegister().flags.n == 0);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xa8);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetYIndex() == 0xaf);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - TYA - Implied - Copy Y Register to Accumulator - zero flag")
{
    Byte test_case[] = {0x98};
    Registers registers{.a = 0xff, .y = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    REQUIRE(cpu.GetStatusRegister().flags.z == 0);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x98);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetAccumulator() == 0x00);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - TYA - Implied - Copy Y Register to Accumulator - negative flag")
{
    Byte test_case[] = {0x98};
    Registers registers{.a = 0x01, .y = 0xaf, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    REQUIRE(cpu.GetStatusRegister().flags.n == 0);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x98);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetAccumulator() == 0xaf);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - CPX - Immediate - Compare Memory and X Index: X >= M")
{
    Byte test_case[] = {0xe0, 0xAF};
    Registers registers{.x = 0xB0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xe0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - CPX - Immediate - Compare Memory and X Index: X == M")
{
    Byte test_case[] = {0xe0, 0xAF};
    Registers registers{.x = 0xAF, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xe0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - CPX - Immediate - Compare Memory and X Index: X < M")
{
    Byte test_case[] = {0xe0, 0xAF};
    Registers registers{.x = 0xAB, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xe0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - CPX - Absolute - Compare Memory and X Index: X >= M")
{
    Byte test_case[] = {0xec, 0x34, 0x12};
    Registers registers{.x = 0xB0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1234, (Byte)0xAF);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xec);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - CPX - Absolute - Compare Memory and X Index: X == M")
{
    Byte test_case[] = {0xec, 0x34, 0x12};
    Registers registers{.x = 0xB0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1234, (Byte)0xB0);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xec);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - CPX - Absolute - Compare Memory and X Index: X < M")
{
    Byte test_case[] = {0xec, 0x34, 0x12};
    Registers registers{.x = 0xAB, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1234, (Byte)0xB0);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xec);
    REQUIRE(cpu.GetStatusRegister().flags.c == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - CPX - Zero Page - Compare Memory and X Index: X >= M")
{
    Byte test_case[] = {0xe4, 0x34};
    Registers registers{.x = 0xB0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x34, (Byte)0xAF);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xe4);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - CPX - Zero Page - Compare Memory and X Index: X == M")
{
    Byte test_case[] = {0xe4, 0x34};
    Registers registers{.x = 0xB0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x34, (Byte)0xB0);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xe4);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - CPX - Zero Page - Compare Memory and X Index: X < M")
{
    Byte test_case[] = {0xe4, 0x34};
    Registers registers{.x = 0xAB, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x34, (Byte)0xAF);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xe4);
    REQUIRE(cpu.GetStatusRegister().flags.c == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - CPY - Immediate - Compare Memory and Y Index: Y >= M")
{
    Byte test_case[] = {0xc0, 0xAF};
    Registers registers{.y = 0xB0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xc0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - CPY - Immediate - Compare Memory and Y Index: Y == M")
{
    Byte test_case[] = {0xc0, 0xAF};
    Registers registers{.y = 0xAF, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xc0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - CPY - Immediate - Compare Memory and Y Index: Y < M")
{
    Byte test_case[] = {0xc0, 0xAF};
    Registers registers{.y = 0xAB, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xc0);
    REQUIRE(cpu.GetStatusRegister().flags.c == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - CPY - Absolute - Compare Memory and Y Index: Y >= M")
{
    Byte test_case[] = {0xcc, 0x34, 0x12};
    Registers registers{.y = 0xB0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1234, (Byte)0xAF);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xcc);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - CPY - Absolute - Compare Memory and Y Index: Y == M")
{
    Byte test_case[] = {0xcc, 0x34, 0x12};
    Registers registers{.y = 0xB0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1234, (Byte)0xB0);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xcc);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - CPY - Absolute - Compare Memory and Y Index: Y < M")
{
    Byte test_case[] = {0xcc, 0x34, 0x12};
    Registers registers{.y = 0xAB, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x1234, (Byte)0xB0);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xcc);
    REQUIRE(cpu.GetStatusRegister().flags.c == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - CPY - Zero Page - Compare Memory and Y Index: Y >= M")
{
    Byte test_case[] = {0xc4, 0x34};
    Registers registers{.y = 0xB0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x34, (Byte)0xAF);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xc4);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - CPY - Zero Page - Compare Memory and Y Index: Y == M")
{
    Byte test_case[] = {0xc4, 0x34};
    Registers registers{.y = 0xB0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x34, (Byte)0xB0);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xc4);
    REQUIRE(cpu.GetStatusRegister().flags.c == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - CPY - Zero Page - Compare Memory and Y Index: Y < M")
{
    Byte test_case[] = {0xc4, 0x34};
    Registers registers{.y = 0xAB, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x34, (Byte)0xAF);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xc4);
    REQUIRE(cpu.GetStatusRegister().flags.c == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - BEQ - Relative - Branch on zero set")
{
    Byte test_case[] = {0xf0, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.z = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0xf0);
    REQUIRE(cpu.GetCycleCount() == 3); // 2 for opcode and 1 for branch
}

TEST_CASE("OpCodes Table - Ops - BEQ - Relative - No Branch on A5")
{
    Byte test_case[] = {0xf0, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.data = 0xa5;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0xf0);
    REQUIRE(registers.sr.flags.z == 0);
    REQUIRE(cpu.GetCycleCount() == 2); // 2 for opcode and 1 for branch
}

TEST_CASE("OpCodes Table - Ops - BEQ - Relative - No branch on zero cleared")
{
    Byte test_case[] = {0xf0, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.z = 0;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0xf0);
    REQUIRE(cpu.GetProgramCounter() == 0x0102);
    REQUIRE(cpu.GetCycleCount() == 2); // 2 for opcode
}

TEST_CASE("OpCodes Table - Ops - BNE - Relative - Branch on zero NOT set")
{
    Byte test_case[] = {0xd0, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.z = 0;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0xd0);
    REQUIRE(cpu.GetProgramCounter() == 0x0102 + 0x12);
    REQUIRE(cpu.GetCycleCount() == 3); // 2 for opcode and 1 for branch
}

TEST_CASE("OpCodes Table - Ops - BNE - Relative - No branch on zero set")
{
    Byte test_case[] = {0xd0, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.z = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0xd0);
    REQUIRE(cpu.GetProgramCounter() == 0x0102);
    REQUIRE(cpu.GetCycleCount() == 2); // 2 for opcode
}

TEST_CASE("OpCodes Table - Ops - BCS - Relative - Branch on carry flag set")
{
    Byte test_case[] = {0xb0, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.c = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0xb0);
    REQUIRE(cpu.GetProgramCounter() == 0x0102 + 0x12);
    REQUIRE(cpu.GetCycleCount() == 3); // 2 for opcode and 1 for branch
}

TEST_CASE("OpCodes Table - Ops - BCS - Relative - No branch on carry cleared")
{
    Byte test_case[] = {0xb0, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.c = 0;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0xb0);
    REQUIRE(cpu.GetProgramCounter() == 0x0102);
    REQUIRE(cpu.GetCycleCount() == 2); // 2 for opcode
}

TEST_CASE("OpCodes Table - Ops - BCC - Relative - Branch on carry flag cleared")
{
    Byte test_case[] = {0x90, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.c = 0;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0x90);
    REQUIRE(cpu.GetProgramCounter() == 0x0102 + 0x12);
    REQUIRE(cpu.GetCycleCount() == 3); // 2 for opcode and 1 for branch
}

TEST_CASE("OpCodes Table - Ops - BCC - Relative - No branch on carry set")
{
    Byte test_case[] = {0x90, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.c = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0x90);
    REQUIRE(cpu.GetProgramCounter() == 0x0102);
    REQUIRE(cpu.GetCycleCount() == 2); // 2 for opcode
}

TEST_CASE("OpCodes Table - Ops - BVS - Relative - Branch on overflow flag set")
{
    Byte test_case[] = {0x70, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.o = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0x70);
    REQUIRE(cpu.GetProgramCounter() == 0x0102 + 0x12);
    REQUIRE(cpu.GetCycleCount() == 3); // 2 for opcode and 1 for branch
}

TEST_CASE("OpCodes Table - Ops - BVS - Relative - No branch on overflow cleared")
{
    Byte test_case[] = {0x70, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.o = 0;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0x70);
    REQUIRE(cpu.GetProgramCounter() == 0x0102);
    REQUIRE(cpu.GetCycleCount() == 2); // 2 for opcode
}

TEST_CASE("OpCodes Table - Ops - BVC - Relative - Branch on overflow flag cleared")
{
    Byte test_case[] = {0x50, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.o = 0;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0x50);
    REQUIRE(cpu.GetProgramCounter() == 0x0102 + 0x12);
    REQUIRE(cpu.GetCycleCount() == 3); // 2 for opcode and 1 for branch
}

TEST_CASE("OpCodes Table - Ops - BVC - Relative - No branch on overflow set")
{
    Byte test_case[] = {0x50, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.o = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0x50);
    REQUIRE(cpu.GetProgramCounter() == 0x0102);
    REQUIRE(cpu.GetCycleCount() == 2); // 2 for opcode
}

TEST_CASE("OpCodes Table - Ops - BMI - Relative - Branch on negative flag set")
{
    Byte test_case[] = {0x30, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.n = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0x30);
    REQUIRE(cpu.GetProgramCounter() == 0x0102 + 0x12);
    REQUIRE(cpu.GetCycleCount() == 3); // 2 for opcode and 1 for branch
}

TEST_CASE("OpCodes Table - Ops - BMI - Relative - No branch on negative cleared")
{
    Byte test_case[] = {0x30, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.n = 0;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0x30);
    REQUIRE(cpu.GetProgramCounter() == 0x0102);
    REQUIRE(cpu.GetCycleCount() == 2); // 2 for opcode
}

TEST_CASE("OpCodes Table - Ops - BPL - Relative - Branch on negative flag cleared")
{
    Byte test_case[] = {0x10, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.n = 0;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0x10);
    REQUIRE(cpu.GetProgramCounter() == 0x0102 + 0x12);
    REQUIRE(cpu.GetCycleCount() == 3); // 2 for opcode and 1 for branch
}

TEST_CASE("OpCodes Table - Ops - BPL - Relative - No branch on negative set")
{
    Byte test_case[] = {0x10, 0x12};
    Registers registers{.pc = 0x0100};
    registers.sr.flags.n = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter(); // pc -> 0x0101

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode); // pc -> 0x0102

    REQUIRE(opcode == 0x10);
    REQUIRE(cpu.GetProgramCounter() == 0x0102);
    REQUIRE(cpu.GetCycleCount() == 2); // 2 for opcode
}

TEST_CASE("OpCodes Table - Ops - INC - Zero Page - Increment memory by one")
{
    Byte test_case[] = {0xe6, 0x12};
    Registers registers{.pc = 0x0600};
    registers.sr.flags.n = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);
    memory.WriteMemory(0x0012, (Byte)0xFF);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xe6);
    REQUIRE(memory.ReadByte(0x0012) == 0x00);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - INC - Absolute - Increment memory by one")
{
    Byte test_case[] = {0xee, 0x75, 0xDE};
    Registers registers{.pc = 0x0600};
    registers.sr.flags.z = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 3);
    memory.WriteMemory(0xDE75, (Byte)0x00);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xee);
    REQUIRE(memory.ReadByte(0xDE75) == 0x01);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - INC - Zero Page X - Increment memory by one")
{
    Byte test_case[] = {0xf6, 0x12};
    Registers registers{.x = 0x72, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);
    memory.WriteMemory(0x0084, (Byte) 0x7f);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xf6);
    REQUIRE(memory.ReadByte(0x0084) == 0x80);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - INC - Absolute X - Increment memory by one")
{
    Byte test_case[] = {0xfe, 0x75, 0xDE};
    Registers registers{.x = 0x01, .pc = 0x0600};
    registers.sr.flags.z = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 3);
    memory.WriteMemory(0xDE76, (Byte) 0x24);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xfe);
    REQUIRE(memory.ReadByte(0xDE76) == 0x25);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetCycleCount() == 7);
}

TEST_CASE("OpCodes Table - Ops - DEC - Zero Page - Decrement memory by one")
{
    Byte test_case[] = {0xc6, 0x12};
    Registers registers{.pc = 0x0600};
    registers.sr.flags.z = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);
    memory.WriteMemory(0x0012, (Byte)0x00);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xc6);
    REQUIRE(memory.ReadByte(0x0012) == 0xFF);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - DEC - Absolute - Decrement memory by one")
{
    Byte test_case[] = {0xce, 0x75, 0xDE};
    Registers registers{.pc = 0x0600};
    registers.sr.flags.n = 1;

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 3);
    memory.WriteMemory(0xDE75, (Byte)0xFF);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xce);
    REQUIRE(memory.ReadByte(0xDE75) == 0xFE);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - DEC - Zero Page X - Decrement memory by one")
{
    Byte test_case[] = {0xd6, 0x12};
    Registers registers{.x = 0x72, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 2);
    memory.WriteMemory(0x0084, (Byte) 0x01);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xd6);
    REQUIRE(memory.ReadByte(0x0084) == 0x00);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetCycleCount() == 6);
}

TEST_CASE("OpCodes Table - Ops - DEC - Absolute X - Decrement memory by one")
{
    Byte test_case[] = {0xde, 0x75, 0xDE};
    Registers registers{.x = 0x01, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(registers.pc, test_case, 3);
    memory.WriteMemory(0xDE76, (Byte) 0x24);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xde);
    REQUIRE(memory.ReadByte(0xDE76) == 0x23);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetCycleCount() == 7);
}

TEST_CASE("OpCodes Table - Ops - DEX - Implied - Decrement Index Register X by one")
{
    Registers registers{.x = 0x00};
    registers.sr.flags.z = 1;

    RawMemoryAccessor memory;

    CPU cpu(registers, &memory);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0xca);

    REQUIRE(cpu.GetXIndex() == 0xFF);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - TXA - Implied - Transfer Index X to Accumulator")
{
    Registers registers{.a = 0x82, .x = 0x00};
    registers.sr.flags.n = 1;

    RawMemoryAccessor memory;

    CPU cpu(registers, &memory);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x8a);

    REQUIRE(cpu.GetXIndex() == 0x00); // Index X shouldn't be affected
    REQUIRE(cpu.GetAccumulator() == 0x00);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - TAX - Implied - Transfer Accumulator to Index X")
{
    Registers registers{.a = 0x82, .x = 0x00};
    registers.sr.flags.z = 1;

    RawMemoryAccessor memory;

    CPU cpu(registers, &memory);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0xaa);

    REQUIRE(cpu.GetAccumulator() == 0x82); // Accumulator shouldn't be affected
    REQUIRE(cpu.GetXIndex() == 0x82);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - TXS- Implied - Transfer Index X to Stack Pointer")
{
    Registers registers{.x = 0x00, .sp = 0xFF};

    RawMemoryAccessor memory;

    CPU cpu(registers, &memory);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x9a);

    REQUIRE(cpu.GetXIndex() == 0x00); // Index X shouldn't be affected
    REQUIRE(cpu.GetStackPointer() == 0x00);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0); // TXS doesn't affect flags
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - TSX- Implied - Transfer Stack Pointer to Index X")
{
    Registers registers{.x = 0x00, .sp = 0xFF};
    registers.sr.flags.z = 1;

    RawMemoryAccessor memory;

    CPU cpu(registers, &memory);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0xba);

    REQUIRE(cpu.GetStackPointer() == 0xFF); // Stack Pointer shouldn't be affected
    REQUIRE(cpu.GetXIndex() == 0xFF);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetCycleCount() == 2);
}


TEST_CASE("OpCodes Table - Ops - JMP - Absolute - jump to new address")
{
    Byte test_case[] = {0x4c, 0x34, 0x12};
    Registers registers{.pc = 0x0100};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 3);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x4c);
    REQUIRE(cpu.GetProgramCounter() == 0x1234);
    REQUIRE(cpu.GetCycleCount() == 3);
}


TEST_CASE("OpCodes Table - Ops - JMP - Absolute Indirect - jump to new address")
{
    Byte test_case[] = {0x6c, 0x34, 0x12};
    Byte test_address[] = {0xCD, 0xAB};
    Registers registers{.pc = 0x0100};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 3);
    memory.WriteMemory(0x1234, test_address, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x6c);
    REQUIRE(cpu.GetProgramCounter() == 0xABCD);
    REQUIRE(cpu.GetCycleCount() == 5);
}

TEST_CASE("OpCodes Table - Ops - BIT - Absolute - Test bits in memory with accumulator, non-zero result")
{
    Byte test_case[] = {0x2c, 0x34, 0x12};
    Byte test_address[] = {0b11100000};
    Registers registers{.a = 0b11000101, .pc = 0x0100};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 3);
    memory.WriteMemory(0x1234, test_address, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x2c);
    REQUIRE(cpu.GetAccumulator() == 0b11000101);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.o == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - BIT - Absolute - Test bits in memory with accumulator, zero result")
{
    Byte test_case[] = {0x2c, 0x34, 0x12};
    Byte test_address[] = {0x00};
    Registers registers{.a = 0b11000101, .pc = 0x0100};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 3);
    memory.WriteMemory(0x1234, test_address, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x2c);
    REQUIRE(cpu.GetAccumulator() == 0b11000101);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.o == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - BIT - Zero Page - Test bits in memory with accumulator, non-zero result")
{
    Byte test_case[] = {0x24, 0x34};
    Byte test_address[] = {0b11100000};
    Registers registers{.a = 0b11000101, .pc = 0x0100};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 3);
    memory.WriteMemory(0x34, test_address, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x24);
    REQUIRE(cpu.GetAccumulator() == 0b11000101);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.o == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 0);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - BIT - Zero Page - Test bits in memory with accumulator, zero result")
{
    Byte test_case[] = {0x24, 0x34};
    Byte test_address[] = {0x00};
    Registers registers{.a = 0b11000101, .pc = 0x0100};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 3);
    memory.WriteMemory(0x34, test_address, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x24);
    REQUIRE(cpu.GetAccumulator() == 0b11000101);
    REQUIRE(cpu.GetStatusRegister().flags.n == 0);
    REQUIRE(cpu.GetStatusRegister().flags.o == 0);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - STY - Zero Page - Store Index Register X In Memory")
{
    Byte test_case[] = {0x84, 0x7C};
    Registers registers{.y = 0x73, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x84);
    REQUIRE(memory.ReadByte(0x007C) == 0x73);
    REQUIRE(cpu.GetCycleCount() == 3);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - STY - Absolute - Store Index Register Y In Memory")
{
    Byte test_case[] = {0x8c, 0xAF, 0x08};
    Registers registers{.y = 0x23, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x8c);
    REQUIRE(memory.ReadByte(0x08AF) == 0x23);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - STY - Zero Page X - Store Index Register Y In Memory")
{
    Byte test_case[] = {0x94, 0xAF};
    Registers registers{.x = 0x10, .y = 0x45, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x94);
    REQUIRE(memory.ReadByte(0x00BF) == 0x45);
    REQUIRE(cpu.GetCycleCount() == 4);
}


TEST_CASE("OpCodes Table - Ops - LDY - Immediate - Load Index Register Y From Memory")
{
    Byte test_case[] = {0xa0, 0xAF};
    Registers registers{.y = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xa0);
    REQUIRE(cpu.GetYIndex() == 0xAF);
    REQUIRE(cpu.GetCycleCount() == 2);
}

TEST_CASE("OpCodes Table - Ops - LDY - Zero Page - Load Index Register Y From Memory")
{
    Byte test_case[] = {0xa4, 0xAF};
    Registers registers{.y = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x00AF, (Byte)0xBC);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xa4);
    REQUIRE(cpu.GetYIndex() == 0xBC);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - LDY - Absolute - Load Index Register Y From Memory")
{
    Byte test_case[] = {0xac, 0xAF, 0x08};
    Registers registers{.y = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x08AF, (Byte)0xA0);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xac);
    REQUIRE(cpu.GetYIndex() == 0xA0);
    REQUIRE(cpu.GetCycleCount() == 4);
}


TEST_CASE("OpCodes Table - Ops - LDY - Zero Page X - Load Index Register Y From Memory")
{
    Byte test_case[] = {0xb4, 0xAF};
    Registers registers{.x = 0x10, .y = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);
    memory.WriteMemory(0x00BF, (Byte) 0xA0);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xb4);
    REQUIRE(cpu.GetYIndex() == 0xA0);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - LDY - Absolute X - Load Index Register Y From Memory")
{
    Byte test_case[] = {0xbc, 0xAF, 0x12};
    Registers registers{.x = 0x21, .y = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 3);
    memory.WriteMemory(0x12D0, (Byte)0x73);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0xbc);
    REQUIRE(cpu.GetYIndex() == 0x73);
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - LDY - Zero flag sets when load zero")
{
    Byte test_case[] = {0xa0, 0x00};
    Registers registers{.y = 0xA0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);
    const auto statusRegister = cpu.GetStatusRegister();
    REQUIRE(statusRegister.flags.z == 1);
}

TEST_CASE("OpCodes Table - Ops - LDY - Zero flag clears when load non-zero")
{
    Byte test_case[] = {0xa0, 0x00, 0xa2, 0x04};
    Registers registers{.y = 0xA0, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 4);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);
    auto statusRegister = cpu.GetStatusRegister();
    REQUIRE(statusRegister.flags.z == 1);

    opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    opcodes.RunOpCode(&cpu, opcode);
    statusRegister = cpu.GetStatusRegister();
    REQUIRE(statusRegister.flags.z == 0);
}

TEST_CASE("OpCodes Table - Ops - LDY - Negative flag sets when load negative")
{
    Byte test_case[] = {0xa0, 0xA0};
    Registers registers{.y = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 2);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);
    const auto statusRegister = cpu.GetStatusRegister();
    REQUIRE(statusRegister.flags.n == 1);
}

TEST_CASE("OpCodes Table - Ops - LDY - Negative flag clears when load non-negative")
{
    Byte test_case[] = {0xa0, 0xA0, 0xa2, 0x00};
    Registers registers{.y = 0x00, .pc = 0x0600};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0600, test_case, 4);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);
    auto statusRegister = cpu.GetStatusRegister();
    REQUIRE(statusRegister.flags.n == 1);

    opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    opcodes.RunOpCode(&cpu, opcode);
    statusRegister = cpu.GetStatusRegister();
    REQUIRE(statusRegister.flags.n == 0);
}

TEST_CASE("OpCodes Table - Ops - PHP - Implied - Push status register on stack")
{
    RawMemoryAccessor memory;
    Registers registers{.x = 0x05, .y = 0xF2, .sp = 0xFF}; // test data
    registers.sr.data = 0x6F;
    CPU cpu(registers, &memory);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x08);

    // check top of stack
    REQUIRE(cpu.GetMemoryByte(0x100 + (cpu.GetStackPointer() + 1)) == 0x7F);

    // sp decremented once
    REQUIRE(cpu.GetStackPointer() == 0xFF - 1);

    // cpu cycle count should increase by 3
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - PLA - Implied - Store top of stack in accumulator")
{
    RawMemoryAccessor memory;
    Registers registers{.a = 0x4D, .sp = 0xFE}; // test data
    registers.sr.data = 0x2f;
    CPU cpu(registers, &memory);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x08); // PHP - push sr on stack with b flag set
    opcodes.RunOpCode(&cpu, 0x68); // PLA

    // check accumulator for new val
    REQUIRE(cpu.GetAccumulator() == 0x3f);

    // sp incremented once
    REQUIRE(cpu.GetStackPointer() == 0xFE);

    // cpu cycle count should increase by 7 - PHP + PLA
    REQUIRE(cpu.GetCycleCount() == 7);
}

TEST_CASE("OpCodes Table - Ops - PLP - Implied - Store top of stack in status register")
{
    RawMemoryAccessor memory;
    Registers registers{.a = 0x04, .sp = 0xFE};
    CPU cpu(registers, &memory);


    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x48); // PHA
    opcodes.RunOpCode(&cpu, 0x28); // PLP

    // check updated status register
    REQUIRE(cpu.GetStatusRegister().data == 0x24);

    // sp incremented once
    REQUIRE(cpu.GetStackPointer() == 0xFE);

    // cpu cycle count should increase by 4
    REQUIRE(cpu.GetCycleCount() == 7);
}

TEST_CASE("OpCodes Table - Ops - ROM PHA PLP")
{
    RawMemoryAccessor memory;
    Registers registers{.a = 0xff, .sp = 0xFB};
    registers.sr.data = 0xad;
    CPU cpu(registers, &memory);


    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x48); // PHA
    opcodes.RunOpCode(&cpu, 0x28); // PLP

    REQUIRE(cpu.GetStatusRegister().data == 0xef);
}

TEST_CASE("OpCodes Table - ROM - BIT - Zero Page - Test bits in memory with accumulator, zero result")
{
    Byte test_case[] = {0x24, 0x01};
    Byte test_address[] = {0xff};
    Registers registers{.a = 0x00, .pc = 0x0100};

    RawMemoryAccessor memory;
    memory.WriteMemory(0x0100, test_case, 2);
    memory.WriteMemory(0x01, test_address, 1);

    CPU cpu(registers, &memory);
    auto opcode = cpu.GetCurrentOpCode();
    cpu.AdvanceProgramCounter();

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, opcode);

    REQUIRE(opcode == 0x24);
    REQUIRE(cpu.GetAccumulator() == 0x00);
    REQUIRE(cpu.GetStatusRegister().flags.n == 1);
    REQUIRE(cpu.GetStatusRegister().flags.o == 1);
    REQUIRE(cpu.GetStatusRegister().flags.z == 1);
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - ROM - RTI - Implied - Return from Interrupt ")
{
    RawMemoryAccessor memory;
    Byte top_of_stack = 0xFF;
    Registers registers{.sp = (Byte)(top_of_stack - 3)};
    CPU cpu(registers, &memory);
    // test data
    Byte new_sr = 0x87;
    Byte pc_l = 0x50;
    Byte pc_h = 0x80;

    // write test data
    memory.WriteMemory(0x100 + top_of_stack, (Byte)pc_h);
    memory.WriteMemory(0x100 + (top_of_stack - 1), (Byte)pc_l);
    memory.WriteMemory(0x100 + (top_of_stack - 2), (Byte)new_sr);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x40);

    // check updated status register
    REQUIRE(cpu.GetStatusRegister().data == 0xA7);

    // sp incremented 3 times
    REQUIRE(cpu.GetStackPointer() == top_of_stack);

    // pc = 0x5080
    REQUIRE(cpu.GetProgramCounter() == 0x8050);

    // cpu cycle count should increase by 6
    REQUIRE(cpu.GetCycleCount() == 6);
}
