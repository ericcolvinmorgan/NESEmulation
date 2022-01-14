#pragma once

#include "constants.h"

class CPU
{
private:
    BYTE _a = 0;  // Accumulator
    BYTE _x = 0;  // X Index
    BYTE _y = 0;  // Y Index
    BYTE _sp = 0; // Stack Pointer
    BYTE _sr = 0; // Status Register
    WORD _pc = 0; // Program Counter

public:
    CPU(){};
    ~CPU(){};
    WORD getProgramCounter() { return _pc; }
    void advanceProgramCounter();
};
