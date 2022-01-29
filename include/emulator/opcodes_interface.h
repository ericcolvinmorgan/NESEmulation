#pragma once
#include "constants.h"
#include "cpu.h"

class OpCodesInterface
{
public:
    virtual ~OpCodesInterface() {};
    virtual uint8_t RunOpCode(CPU *cpu, Byte opcode) = 0;
};