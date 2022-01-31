#pragma once

#include "cpu.h"
#include "opcodes_interface.h"

class Emulator
{
private:
    const int cpu_cycles_per_frame_ = 29780;
    CPU *cpu_;
    OpCodesInterface *cpu_opcodes_;
    int frame_ = 0;

public:
    Emulator(CPU *cpu, OpCodesInterface *opcodes)
    {
        cpu_ = cpu;
        cpu_opcodes_ = opcodes;
    };
    ~Emulator(){};
    int GetFrame() { return frame_; };
    void AdvanceFrame();
};