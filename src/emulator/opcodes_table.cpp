#include "../../include/emulator/opcodes_table.h"

    /*
    TODO - OPEN ITEMS:
    
    1) I'm a little unclear how to handle wrap around when we exceed FFFF.
    https://softpixel.com/~cwright/sianse/docs/65816NFO.HTM
    For absolute indexed, this resource seems to indicate we'd wrap around 
    into the zero page, though I know I've seen other resources that seem 
    to extend past FFFF and not wrap.
    
    */

OpCodesTable::OpCodesTable()
{
    for (int i = 0; i <= 0xFF; i++)
        opcodes_[i] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;

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
    return {absolute_address, true};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeAbsoluteX(CPU *cpu)
{
    cpu->IncreaseCycleCount(4);
    auto absolute_address = cpu->GetMemoryWord(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    cpu->AdvanceProgramCounter();
    uint32_t indexed_address = (absolute_address + cpu->GetXIndex());
    
    //Crossed page boundry increasing cycle count by 1.
    if((indexed_address >> 8) > (absolute_address >> 8))
        cpu->IncreaseCycleCount(1);

    return {(uint16_t)(indexed_address & 0x0000FFFF), true};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeAbsoluteY(CPU *cpu)
{
    cpu->IncreaseCycleCount(4);
    auto absolute_address = cpu->GetMemoryWord(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    cpu->AdvanceProgramCounter();
    uint32_t indexed_address = (absolute_address + cpu->GetYIndex());
    
    //Crossed page boundry increasing cycle count by 1.
    if((indexed_address >> 8) > (absolute_address >> 8))
        cpu->IncreaseCycleCount(1);

    return {(uint16_t)(indexed_address & 0x0000FFFF), true};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeZeroPage(CPU *cpu)
{
    cpu->IncreaseCycleCount(3);
    auto zero_page_addr = cpu->GetMemoryByte(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    return {zero_page_addr, true};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeZeroPageX(CPU *cpu)
{
    cpu->IncreaseCycleCount(4);
    Word zero_page_addr = cpu->GetMemoryByte(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    zero_page_addr = (zero_page_addr + cpu->GetXIndex()) & 0x00FF;
    return {zero_page_addr, true};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeZeroPageY(CPU *cpu)
{
    cpu->IncreaseCycleCount(4);
    Word zero_page_addr = cpu->GetMemoryByte(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    zero_page_addr = (zero_page_addr + cpu->GetYIndex()) & 0x00FF;
    return {zero_page_addr, true};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeIndirectX(CPU *cpu)
{
    cpu->IncreaseCycleCount(2);
    auto immediate_val = cpu->GetCurrentOpCode();
    cpu->AdvanceProgramCounter();
    return {immediate_val, true};
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
    cpu->IncreaseCycleCount(2);
    Byte offset = cpu->GetMemoryByte(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    uint16_t program_counter = cpu->GetProgramCounter();
    uint16_t relative_address = program_counter + ((int8_t)offset);
    if(((program_counter ^ relative_address) & 0xFF00) != 0x0000)
        cpu->IncreaseCycleCount(1);        
    return {relative_address, false};
}

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpNotImplemented(CPU *cpu, Byte opcode)
{
    auto address_mode_val = ((*this).*A)(cpu);
    cpu->AdvanceProgramCounter();
};

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
