#pragma once
#include "opcodes_interface.h"
#include "cpu.h"

class OpCodesTable : public OpCodesInterface
{
    struct AddressingVal
    {
        uint16_t value;
        bool is_address;
        uint8_t cycles;
    };

private:
    typedef struct AddressingVal (OpCodesTable::*AddressMode)(CPU *);
    typedef void (OpCodesTable::*OpCodeFunction)(CPU *, Byte);
    OpCodeFunction opcodes_[0xFF + 1] = {0};

    // Helpers
    inline void UpdateNegativeFlag(CPU *cpu, uint8_t result);
    inline void UpdateZeroFlag(CPU *cpu, uint8_t result);
    inline void UpdateCarryFlag(CPU *cpu, uint16_t result);
    inline void UpdateOverflowFlag(CPU *cpu, Byte a, Byte m, Byte r);

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
    template <OpCodesTable::AddressMode A>
    void OpADC(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpAND(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpBRK(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpPHP(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpPHA(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpPLA(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpPLP(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpRTI(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpRTS(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpJSR(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpCMP(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpEOR(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpLDA(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpORA(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpSBC(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpSTA(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpLDX(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpSTX(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpSED(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpSEI(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpSEC(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpCLD(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpCLV(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpCLI(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpCLC(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpASL(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpLSR(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpROL(CPU *cpu, Byte opcode);
    template <OpCodesTable::AddressMode A>
    void OpROR(CPU *cpu, Byte opcode);
};