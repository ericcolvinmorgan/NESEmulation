#include "../../include/emulator/opcodes_table.h"

OpCodesTable::OpCodesTable()
{
    for (int i = 0; i <= 0xFF; i++)
        opcodes_[i] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
}

uint8_t OpCodesTable::RunOpCode(CPU *cpu, Byte opcode)
{
    ((*this).*(opcodes_[opcode]))(cpu, opcode);
    return 0;
}

uint16_t OpCodesTable::AddressingModeImplied(CPU *cpu)
{
    cpu->IncreaseCycleCount(2);
    return 0;
}

uint16_t OpCodesTable::AddressingModeAccumulator(CPU *cpu)
{
    cpu->IncreaseCycleCount(2);
    return cpu->GetAccumulator();
}

uint16_t OpCodesTable::AddressingModeImmediate(CPU *cpu)
{
    cpu->IncreaseCycleCount(2);
    auto immediateVal = cpu->GetCurrentOpCode();
    cpu->AdvanceProgramCounter();
    return immediateVal;
}

uint16_t OpCodesTable::AddressingModeAbsolute(CPU *cpu)
{
    return 0;
}

uint16_t OpCodesTable::AddressingModeAbsoluteX(CPU *cpu)
{
    return 0;
}

uint16_t OpCodesTable::AddressingModeAbsoluteY(CPU *cpu)
{
    return 0;
}

uint16_t OpCodesTable::AddressingModeZeroPage(CPU *cpu)
{
    cpu->IncreaseCycleCount(3);
    return 0;
}

uint16_t OpCodesTable::AddressingModeZeroPageX(CPU *cpu)
{
    cpu->IncreaseCycleCount(4);
    return 0;
}

uint16_t OpCodesTable::AddressingModeZeroPageY(CPU *cpu)
{
    cpu->IncreaseCycleCount(4);
    return 0;
}

uint16_t OpCodesTable::AddressingModeIndirectX(CPU *cpu)
{
    return 0;
}

uint16_t OpCodesTable::AddressingModeIndirectY(CPU *cpu)
{
    return 0;
}

uint16_t OpCodesTable::AddressingModeAbsoluteIndirect(CPU *cpu)
{
    return 0;
}

uint16_t OpCodesTable::AddressingModeRelative(CPU *cpu)
{
    return 0;
}

template <OpCodesTable::AddressMode addrMode>
void OpCodesTable::OpNotImplemented(CPU *cpu, Byte opcode)
{
    uint16_t v = ((*this).*addrMode)(cpu);
    cpu->AdvanceProgramCounter();
};
