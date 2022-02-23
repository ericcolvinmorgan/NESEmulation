#include "../../include/emulator/cpu.h"
#include "../../include/emulator/constants.h"

CPU::CPU(MemoryAccessorInterface *memory) { memory_ = memory; };

CPU::CPU(Registers registers, MemoryAccessorInterface *memory)
{
    registers_ = registers;
    memory_ = memory;
};

void CPU::AdvanceProgramCounter()
{
    if(registers_.pc < 0xFFFF)
        registers_.pc++;
}

void CPU::Reset()
{
    registers_.pc = memory_->ReadWord(kReset);
}

//Handle nom-maskable interrupts
void CPU::Interrupt()
{
    // TODO - This is just copied from the opcode BRK implementation.  Can we just reuse or does anything change?  Open for further investiation.
    Byte pc_h = (GetProgramCounter() & 0xFF00) >> 8;
    Byte pc_l = GetProgramCounter() & 0xFF;

    WriteMemory(0x100 + GetStackPointer(), pc_h);
    DecrementStackPointer();

    WriteMemory(0x100 + GetStackPointer(), pc_l);
    DecrementStackPointer();

    WriteMemory(0x100 + GetStackPointer(), GetStatusRegister().data);
    DecrementStackPointer();

    Word new_pc = GetMemoryWord(0xFFFA); // This changed from the BRK implementation to point to the NMI vector
    SetProgramCounter(new_pc);

    IncreaseCycleCount(7); // 7 cycles total
}