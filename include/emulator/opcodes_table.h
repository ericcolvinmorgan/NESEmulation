#pragma once
#include "opcodes_interface.h"
#include "cpu.h"

class OpCodesTable : public OpCodesInterface
{
private:
    typedef void (OpCodesTable::*OpCodeFunction)(CPU *, Byte);

    struct OpCodesDef
    {
        OpCodeFunction opcode_func;
        uint8_t bytes;
        uint8_t cycles;
    };

    struct OpCodesDef opcodes_[0xFF + 1] = {0};

public:
    OpCodesTable();
    uint8_t RunOpCode(CPU *cpu, Byte opcode);
    void NotImplemented(CPU *cpu, Byte opcode)
    {
        cpu->AdvanceProgramCounter();
    };
};