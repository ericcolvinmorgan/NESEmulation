#pragma once

#include "cpu.h"
#include "opcodes_interface.h"
#include "ppu.h"

class Emulator
{
private:
    int cpu_cycles_per_frame_;
    PPU *ppu_;
    CPU *cpu_;
    OpCodesInterface *cpu_opcodes_;
    int frame_ = 0;
    bool enable_logging_;

public:
    Emulator(PPU *ppu, CPU *cpu, OpCodesInterface *opcodes, int cpu_cycles_per_frame = 29780)
    {
        ppu_ = ppu;
        cpu_ = cpu;
        cpu_opcodes_ = opcodes;
        cpu_cycles_per_frame_ = cpu_cycles_per_frame;
    };
    ~Emulator(){};
    int GetFrame() { return frame_; };
    void AdvanceFrame();
    void EnableLogging(bool enable_logging) { enable_logging_ = enable_logging; };
};