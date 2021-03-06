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
    uint32_t frame_ = 0;
    bool enable_logging_ = false;

public:
    Emulator(PPU *ppu, CPU *cpu, OpCodesInterface *opcodes, int cpu_cycles_per_frame = 29780)
    {
        ppu_ = ppu;
        cpu_ = cpu;
        cpu_opcodes_ = opcodes;
        cpu_cycles_per_frame_ = cpu_cycles_per_frame;
    };
    ~Emulator(){};
    uint32_t GetFrame() { return frame_; };
    void AdvanceFrame();
    void EnableLogging(bool enable_logging) { enable_logging_ = enable_logging; };
};