#include "../../include/emulator/cpu.h"
#include "../../include/emulator/constants.h"

void CPU::AdvanceProgramCounter()
{
    pc_++;
}

void CPU::Reset()
{
    pc_ = memory_->ReadWord(kReset);
}