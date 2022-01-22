#pragma once
#include "opcodes_interface.h"
#include "cpu.h"

class OpCodesTable : public OpCodesInterface
{
    struct AddressingVal
    {
        uint16_t value;
        bool is_address;
    };

private:
    typedef struct AddressingVal (OpCodesTable::*AddressMode)(CPU *);
    typedef void (OpCodesTable::*OpCodeFunction)(CPU *, Byte);
    OpCodeFunction opcodes_[0xFF + 1] = {0};

public:
    OpCodesTable();
    uint8_t RunOpCode(CPU *cpu, Byte opcode);

    // Addressing Modes
    AddressingVal AddressingModeNone(CPU *cpu) { return {0, false}; };
    AddressingVal AddressingModeImplied(CPU *cpu);
    AddressingVal AddressingModeAccumulator(CPU *cpu);
    AddressingVal AddressingModeImmediate(CPU *cpu);
    AddressingVal AddressingModeAbsolute(CPU *cpu);
    AddressingVal AddressingModeAbsoluteX(CPU *cpu);
    AddressingVal AddressingModeAbsoluteY(CPU *cpu);
    AddressingVal AddressingModeZeroPage(CPU *cpu);
    AddressingVal AddressingModeZeroPageX(CPU *cpu);
    AddressingVal AddressingModeZeroPageY(CPU *cpu);
    AddressingVal AddressingModeIndirectX(CPU *cpu);
    AddressingVal AddressingModeIndirectY(CPU *cpu);
    AddressingVal AddressingModeAbsoluteIndirect(CPU *cpu);
    AddressingVal AddressingModeRelative(CPU *cpu);

    // Implemented Op Codes
    template <OpCodesTable::AddressMode A>
    void OpNotImplemented(CPU *cpu, Byte opcode);
    void OpBRK(CPU *cpu, Byte opcode);
    void OpPHP(CPU *cpu, Byte opcode);
    void OpPHA(CPU *cpu, Byte opcode);
    void OpPLA(CPU *cpu, Byte opcode);
    void OpPLP(CPU *cpu, Byte opcode);
    void OpRTI(CPU *cpu, Byte opcode);
    void OpRTS(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpLDA(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpSTA(CPU *cpu, Byte opcode);
};