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
    cpu.WriteMemory((Word)0xFFFE, (Byte)0x55);
    cpu.WriteMemory((Word)0xFFFF, (Byte)0xEE);
    
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

TEST_CASE("OpCodes Table - Ops - PHP - Implied - Push status register on stack")
{
    RawMemoryAccessor memory;
    Registers registers {.x = 0x05, .y = 0xF2, .sp = 0xFF}; // test data
    CPU cpu(registers, &memory);
    
    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x08);
    
    // check top of stack
    REQUIRE(cpu.GetMemoryByte(0x100 + (cpu.GetStackPointer() + 1)) == cpu.GetStatusRegister().data);
    
    // sp decremented once
    REQUIRE(cpu.GetStackPointer() == 0xFF - 1);

    // cpu cycle count should increase by 3
    REQUIRE(cpu.GetCycleCount() == 3);
}

TEST_CASE("OpCodes Table - Ops - PHA - Implied - Push accumulator on stack")
{
    RawMemoryAccessor memory;
    Registers registers {.a = 0x4D, .sp = 0xFF}; // test data
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

TEST_CASE("OpCodes Table - Ops - PLA - Implied - Store top of stack in accumulator")
{
    RawMemoryAccessor memory;
    Registers registers {.a = 0x4D, .sp = 0xFE}; // test data
    CPU cpu(registers, &memory);
    Byte new_accumulator = 0x3D;
    // write test data to top of stack
    cpu.WriteMemory(0x100 + (cpu.GetStackPointer() + 1), (Byte)new_accumulator);
    
    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x68);
    
    // check accumulator for new val
    REQUIRE(cpu.GetAccumulator() == new_accumulator);
    
    // sp incremented once
    REQUIRE(cpu.GetStackPointer() == 0xFE + 1);

    // cpu cycle count should increase by 4
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - PLP - Implied - Store top of stack in status register")
{
    RawMemoryAccessor memory;
    Registers registers {.sp = 0xFE}; 
    CPU cpu(registers, &memory);
    Byte new_sr = 0b10101011;   // test data
    
    // write test data to top of stack
    cpu.WriteMemory(0x100 + cpu.GetStackPointer() + 1, (Byte)new_sr);
    
    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x28);
    
    // check updated status register
    REQUIRE(cpu.GetStatusRegister().data == new_sr);
    
    // sp incremented once
    REQUIRE(cpu.GetStackPointer() == 0xFE + 1);

    // cpu cycle count should increase by 4
    REQUIRE(cpu.GetCycleCount() == 4);
}

TEST_CASE("OpCodes Table - Ops - RTI - Implied - Return from Interrupt ")
{
    RawMemoryAccessor memory;
    Byte top_of_stack = 0xFF;
    Registers registers { .sp = (Byte)(top_of_stack - 3) };
    CPU cpu(registers, &memory);
    // test data
    Byte new_sr = 0b10101011;  
    Byte pc_l = 0x50;
    Byte pc_h = 0x80; 
    
    // write test data
    memory.WriteMemory(0x100 + top_of_stack, (Byte) pc_h);
    memory.WriteMemory(0x100 + (top_of_stack - 1), (Byte) pc_l);
    memory.WriteMemory(0x100 + (top_of_stack - 2), (Byte) new_sr);
    
    
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
    Registers registers { .sp = (Byte)(top_of_stack - 2) };
    CPU cpu(registers, &memory);
    // test data
    Byte pc_l = 0x02;
    Byte pc_h = 0x01; 
    
    // write test data
    memory.WriteMemory(0x100 + top_of_stack, (Byte) pc_h);
    memory.WriteMemory(0x100 + (top_of_stack - 1), (Byte) pc_l);
    
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
    Registers registers { .sp = top_of_stack, .pc = 0x0101 };
    CPU cpu(registers, &memory);
    // test data
    Byte hi = 0x12;
    Byte lo = 0x34;
    
    // write test data
    cpu.WriteMemory(0x0101, (Byte) lo);
    cpu.WriteMemory(0x0102, (Byte) hi);

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
    Registers registers { .sp = top_of_stack, .pc = 0x0101 };
    CPU cpu(registers, &memory);
    // test data
    Byte hi = 0x12;
    Byte lo = 0x34;
    
    // write test data
    cpu.WriteMemory(0x0101, (Byte) lo);
    cpu.WriteMemory(0x0102, (Byte) hi);

    OpCodesTable opcodes;
    opcodes.RunOpCode(&cpu, 0x20);  // JSR
    opcodes.RunOpCode(&cpu, 0x60);  // RTS

    // stack should be empty again
    REQUIRE(cpu.GetStackPointer() == top_of_stack);

    REQUIRE(cpu.GetProgramCounter() == 0x103);
    

}
