#pragma once
#include "constants.h"
#include "cpu.h"

class IOpCodes
{
public:
    virtual uint8_t runOpCode(CPU *cpu, BYTE opcode) = 0;
};