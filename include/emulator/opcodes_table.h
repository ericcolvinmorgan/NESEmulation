#pragma once
#include "opcodes_interface.h"
#include "cpu.h"

class OpCodesTable : public OpCodesInterface
{
private:
    typedef uint16_t (OpCodesTable::*AddressMode)(CPU *);
    typedef void (OpCodesTable::*OpCodeFunction)(CPU *, Byte);
    OpCodeFunction opcodes_[0xFF + 1] = {0};

public:
    OpCodesTable();
    uint8_t RunOpCode(CPU *cpu, Byte opcode);

    // Addressing Modes
    uint16_t AddressingModeNone(CPU *cpu) { return 0; };
    uint16_t AddressingModeImplied(CPU *cpu);
    uint16_t AddressingModeAccumulator(CPU *cpu);
    uint16_t AddressingModeImmediate(CPU *cpu);
    uint16_t AddressingModeAbsolute(CPU *cpu);
    uint16_t AddressingModeAbsoluteX(CPU *cpu);
    uint16_t AddressingModeAbsoluteY(CPU *cpu);
    uint16_t AddressingModeZeroPage(CPU *cpu);
    uint16_t AddressingModeZeroPageX(CPU *cpu);
    uint16_t AddressingModeZeroPageY(CPU *cpu);
    uint16_t AddressingModeIndirectX(CPU *cpu);
    uint16_t AddressingModeIndirectY(CPU *cpu);
    uint16_t AddressingModeAbsoluteIndirect(CPU *cpu);
    uint16_t AddressingModeRelative(CPU *cpu);
    
    // Implemented Op Codes
    template <AddressMode addrMode> void OpNotImplemented(CPU *cpu, Byte opcode);
};