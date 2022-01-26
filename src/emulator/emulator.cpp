#include <iostream>
#include "../../include/emulator/emulator.h"

void Emulator::AdvanceFrame()
{
    //Advance CPU
    while(cpu_->GetCycleCount() < cpu_cycles_per_frame_)
    {
            auto opcode = cpu_->GetCurrentOpCode();
            cpu_->AdvanceProgramCounter();
            cpu_opcodes_->RunOpCode(cpu_, opcode);
    }
    cpu_->ResetCycleCount();
}