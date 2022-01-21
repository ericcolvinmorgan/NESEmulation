#include "../../include/emulator/opcodes_table.h"

OpCodesTable::OpCodesTable()
{
    for (int i = 0; i <= 0xFF; i++)
        opcodes_[i] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;

    opcodes_[0x00] = &OpCodesTable::OpBRK;
    opcodes_[0x8d] = &OpCodesTable::OpSTA<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0xa9] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeImmediate>;
}

uint8_t OpCodesTable::RunOpCode(CPU *cpu, Byte opcode)
{
    ((*this).*(opcodes_[opcode]))(cpu, opcode);
    return 0;
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeImplied(CPU *cpu)
{
    cpu->IncreaseCycleCount(2);
    return {0, false};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeAccumulator(CPU *cpu)
{
    cpu->IncreaseCycleCount(2);
    return {cpu->GetAccumulator(), true};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeImmediate(CPU *cpu)
{
    cpu->IncreaseCycleCount(2);
    auto immediate_val = cpu->GetCurrentOpCode();
    cpu->AdvanceProgramCounter();
    return {immediate_val, false};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeAbsolute(CPU *cpu)
{
    cpu->IncreaseCycleCount(4);
    auto absolute_address = cpu->GetMemoryWord(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    cpu->AdvanceProgramCounter();
    return {absolute_address, false};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeAbsoluteX(CPU *cpu)
{
    return {0, false};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeAbsoluteY(CPU *cpu)
{
    return {0, false};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeZeroPage(CPU *cpu)
{
    cpu->IncreaseCycleCount(3);
    return {0, false};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeZeroPageX(CPU *cpu)
{
    cpu->IncreaseCycleCount(4);
    return {0, false};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeZeroPageY(CPU *cpu)
{
    cpu->IncreaseCycleCount(4);
    return {0, false};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeIndirectX(CPU *cpu)
{
    return {0, false};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeIndirectY(CPU *cpu)
{
    return {0, false};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeAbsoluteIndirect(CPU *cpu)
{
    return {0, false};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeRelative(CPU *cpu)
{
    return {0, false};
}

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpNotImplemented(CPU *cpu, Byte opcode)
{
    auto address_mode_val = ((*this).*A)(cpu);
    cpu->AdvanceProgramCounter();
};

// BRK
// cycle 1: fetch opcode, increment pc_
// cycle 2: read next instruction byte(unused), increment pc_
// cycle 3: push PCH on stack with B flag set, decrement sp_
// cycle 4: puch PCL on stack, decrement sp_
// cycle 5: push status register on stack, decrement sp_
// cycle 6: fetch PCL at $FFFE
// cycle 7: fetch PCH at $FFFF
void OpCodesTable::OpBRK(CPU *cpu, Byte opcode)
{
    Byte b_flag = 0b00110000; // bits 4 and 5 set BRK
    cpu->SetStatusRegisterFlag(b_flag);

    Byte pc_h = (cpu->GetProgramCounter() & 0xFF00) >> 8;
    Byte pc_l = cpu->GetProgramCounter() & 0xFF;

    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), pc_h); 
    cpu->DecrementStackPointer();

    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), pc_l);
    cpu->DecrementStackPointer();

    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), cpu->GetStatusRegister().data);
    cpu->DecrementStackPointer();

    pc_l = cpu->GetMemoryByte(0xFFFE);
    pc_h = cpu->GetMemoryByte(0xFFFF);
    cpu->SetProgramCounter((pc_h << 8) | pc_l);

    cpu->IncreaseCycleCount(7);
}

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpLDA(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    if (address_mode_val.is_address)
        address_mode_val.value = cpu->GetMemoryWord(address_mode_val.value);

    cpu->SetAccumulator(address_mode_val.value);
};

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpSTA(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->WriteMemory(address_mode_val.value, cpu->GetAccumulator());
};
