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

struct Registers
{
    Byte a = 0;                                      // Accumulator
    Byte x = 0;                                      // X Index
    Byte y = 0;                                      // Y Index
    Byte sp = 0;                                     // Stack Pointer
    struct StatusRegister sr = {.data = 0b00000100}; // Status Register
    Word pc = 0;                                     // Program Counter
};

class CPU
{
private:
    Registers registers_;
    uint32_t cycle_count_ = 0;
    MemoryAccessorInterface *memory_;

public:
    CPU(MemoryAccessorInterface *memory);
    CPU(Registers registers, MemoryAccessorInterface *memory);
    ~CPU(){};
    void AdvanceProgramCounter();
    Byte GetAccumulator() { return registers_.a; };
    uint32_t GetCycleCount() { return cycle_count_; };
    Byte GetCurrentOpCode() { return memory_->ReadByte(registers_.pc); };
    Word GetProgramCounter() { return registers_.pc; };
    const struct Registers *GetRegistersSnapshot() { return &registers_; };
    void IncreaseCycleCount(uint32_t cycles) { cycle_count_ += cycles; };
    void Reset();
    void ResetCycleCount() { cycle_count_ = 0; };
};
