#pragma once
#include "iopcodes.h"
#include "cpu.h"

class OpCodesTable : public IOpCodes
{
private:
    typedef void (OpCodesTable::*opcodeFunction)(CPU *, BYTE);

    struct OpCodesDef
    {
        opcodeFunction opcodeFunc;
        uint8_t bytes;
        uint8_t cycles;
    };

    struct OpCodesDef _opcodes[0xFF + 1] = {0};

public:
    OpCodesTable();
    uint8_t runOpCode(CPU *cpu, BYTE opcode);
    void notImplemented(CPU *cpu, BYTE opcode)
    {
        cpu->advanceProgramCounter();
    };
};