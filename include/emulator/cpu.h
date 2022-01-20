#pragma once

#include "constants.h"
#include "memory_accessor_interface.h"

struct Flags
{
    Byte n : 1; // Negative Flag
    Byte o : 1; // Overflow Flag
    Byte : 1;   // Unused - Documentation indicates this seems to always be set to 1.
    Byte b : 1; // Break Flag
    Byte d : 1; // Decimal Mode Flag
    Byte i : 1; // Interrupt Disable Flag
    Byte z : 1; // Zero Flag
    Byte c : 1; // Carry Flag
};

struct StatusRegister
{
    union
    {
        struct Flags flags;
        Byte data;
    };
};

class CPU
{
private:
    Byte a_ = 0;                                     // Accumulator
    Byte x_ = 0;                                     // X Index
    Byte y_ = 0;                                     // Y Index
    Byte sp_ = 0;                                    // Stack Pointer
    struct StatusRegister sr_ = {data : 0b00000100}; // Status Register
    Word pc_ = 0;                                    // Program Counter
    MemoryAccessorInterface *memory_;

public:
    CPU(MemoryAccessorInterface *memory){ memory_ = memory; };
    ~CPU(){};
    void AdvanceProgramCounter();
    Word GetProgramCounter() { return pc_; }
    Byte GetCurrentOpCode() { return memory_->ReadByte(pc_); }
    void Reset();
};
