#include <iostream>
#include "../../include/emulator/emulator.h"

void Emulator::AdvanceFrame()
{
    //Advance CPU
    while(cpu_->GetCycleCount() < cpu_cycles_per_frame_)
    {
            auto opcode = cpu_->GetCurrentOpCode();
            // printf("A=$%x X=$%x Y=$%x SP=$%x PC=$%x NV-BDIZC=%d%d%d%d%d%d%d%d OpCode=$%x\n", 
            // cpu_->GetAccumulator(), cpu_->GetXIndex(), cpu_->GetYIndex(), cpu_->GetStackPointer(),
            // cpu_->GetProgramCounter(), 
            // cpu_->GetStatusRegister().flags.n,
            // cpu_->GetStatusRegister().flags.o,
            // 1,
            // cpu_->GetStatusRegister().flags.b,
            // cpu_->GetStatusRegister().flags.d,
            // cpu_->GetStatusRegister().flags.i,
            // cpu_->GetStatusRegister().flags.z,
            // cpu_->GetStatusRegister().flags.c,
            // opcode);

            cpu_->AdvanceProgramCounter();
            cpu_opcodes_->RunOpCode(cpu_, opcode);
    }
    cpu_->ResetCycleCount();
}