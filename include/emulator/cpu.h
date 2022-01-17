#pragma once

#include "constants.h"
#include "imemory_accessor.h"

struct Flags
{
    BYTE n : 1; // Negative Flag
    BYTE o : 1; // Overflow Flag
    BYTE : 1;   // Unused - Documentation indicates this seems to always be set to 1.
    BYTE b : 1; // Break Flag
    BYTE d : 1; // Decimal Mode Flag
    BYTE i : 1; // Interrupt Disable Flag
    BYTE z : 1; // Zero Flag
    BYTE c : 1; // Carry Flag
};

struct StatusRegister
{
    union
    {
        struct Flags flags;
        BYTE data;
    };
};

class CPU
{
private:
    BYTE _a = 0;                                     // Accumulator
    BYTE _x = 0;                                     // X Index
    BYTE _y = 0;                                     // Y Index
    BYTE _sp = 0;                                    // Stack Pointer
    struct StatusRegister _sr = {data : 0b00000100}; // Status Register
    WORD _pc = 0;                                    // Program Counter

public:
    CPU(IMemoryAccessor *memory){};
    ~CPU(){};
    void advanceProgramCounter();
    WORD getProgramCounter() { return _pc; }
    void reset();
};
