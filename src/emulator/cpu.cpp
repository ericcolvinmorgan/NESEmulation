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
    registers_.pc++;
}

void CPU::Reset()
{
    registers_.pc = memory_->ReadWord(kReset);
}