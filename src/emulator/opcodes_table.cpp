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

    opcodes_[0x00] = &OpCodesTable::OpBRK<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x08] = &OpCodesTable::OpPHP<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x20] = &OpCodesTable::OpJSR<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x28] = &OpCodesTable::OpPLP<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x40] = &OpCodesTable::OpRTI<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x48] = &OpCodesTable::OpPHA<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x60] = &OpCodesTable::OpRTS<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x68] = &OpCodesTable::OpPLA<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x86] = &OpCodesTable::OpSTX<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x8d] = &OpCodesTable::OpSTA<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x8e] = &OpCodesTable::OpSTX<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x96] = &OpCodesTable::OpSTX<&OpCodesTable::AddressingModeZeroPageY>;
    opcodes_[0xa1] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeIndirectX>;
    opcodes_[0xa2] = &OpCodesTable::OpLDX<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0xa5] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0xa6] = &OpCodesTable::OpLDX<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0xa9] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0xad] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0xae] = &OpCodesTable::OpLDX<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0xb1] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeIndirectY>;
    opcodes_[0xb5] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0xb6] = &OpCodesTable::OpLDX<&OpCodesTable::AddressingModeZeroPageY>;
    opcodes_[0xb9] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeAbsoluteY>;
    opcodes_[0xbd] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0xbe] = &OpCodesTable::OpLDX<&OpCodesTable::AddressingModeAbsoluteY>;
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

    // Crossed page boundry increasing cycle count by 1.
    if ((indexed_address >> 8) > (absolute_address >> 8))
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

    // Crossed page boundry increasing cycle count by 1.
    if ((indexed_address >> 8) > (absolute_address >> 8))
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
    cpu->IncreaseCycleCount(6);
    Byte indirect_addr = cpu->GetMemoryByte(cpu->GetProgramCounter());
    Word addr = cpu->GetMemoryWord((indirect_addr + cpu->GetXIndex()) & 0x00FF);
    cpu->AdvanceProgramCounter();
    return {addr, true};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeIndirectY(CPU *cpu)
{
    cpu->IncreaseCycleCount(5);
    Byte indirect_addr = cpu->GetMemoryByte(cpu->GetProgramCounter());
    Word dereferenced_addr = cpu->GetMemoryWord(indirect_addr);
    Word addr = dereferenced_addr + cpu->GetYIndex();
    cpu->AdvanceProgramCounter();

    if ((addr >> 8) > (dereferenced_addr >> 8))
        cpu->IncreaseCycleCount(1);

    return {addr, true};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeAbsoluteIndirect(CPU *cpu)
{
    cpu->IncreaseCycleCount(5);
    Word indirect_addr = cpu->GetMemoryWord(cpu->GetProgramCounter());
    Word addr = cpu->GetMemoryWord(indirect_addr);
    cpu->AdvanceProgramCounter();
    cpu->AdvanceProgramCounter();
    return {addr, true};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeRelative(CPU *cpu)
{
    cpu->IncreaseCycleCount(2);
    Byte offset = cpu->GetMemoryByte(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    uint16_t program_counter = cpu->GetProgramCounter();
    uint16_t relative_address = program_counter + ((int8_t)offset);
    if (((program_counter ^ relative_address) & 0xFF00) != 0x0000)
        cpu->IncreaseCycleCount(1);
    return {relative_address, false};
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
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpBRK(CPU *cpu, Byte opcode)
{
    cpu->SetStatusRegisterFlag(kBreakFlag);

    Byte pc_h = (cpu->GetProgramCounter() & 0xFF00) >> 8;
    Byte pc_l = cpu->GetProgramCounter() & 0xFF;

    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), pc_h);
    cpu->DecrementStackPointer();

    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), pc_l);
    cpu->DecrementStackPointer();

    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), cpu->GetStatusRegister().data);
    cpu->DecrementStackPointer();

    Word new_pc = cpu->GetMemoryWord(0xFFFE);
    cpu->SetProgramCounter(new_pc);

    cpu->IncreaseCycleCount(7);
}

// PHP
// Push status register on stack, decrement stack pointer
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpPHP(CPU *cpu, Byte opcode){
    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), cpu->GetStatusRegister().data);
    cpu->DecrementStackPointer();
    cpu->IncreaseCycleCount(3);
}

// PHA
// Push accumulator on stack, decrement stack pointer
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpPHA(CPU *cpu, Byte opcode){
    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), cpu->GetAccumulator());
    cpu->DecrementStackPointer();
    cpu->IncreaseCycleCount(3);
}

// PLA
// increment stack pointer, pull top of stack and store in accumulator
// zero flag set if copied value is 0, otherwise cleared
// negative flag is set to 7th bit of copied value
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpPLA(CPU *cpu, Byte opcode){
    cpu->IncrementStackPointer();
    Byte copied_value = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());
    cpu->SetAccumulator(copied_value);

    // set zero flag
    if (copied_value) {
        cpu->SetStatusRegisterFlag(kZeroFlag);
    } else {
        cpu->ClearStatusRegisterFlag(kZeroFlag);
    }

    // negative flag
    if (copied_value >> 7 == 1) {
        cpu->SetStatusRegisterFlag(kNegativeFlag);
    } else {
        cpu->ClearStatusRegisterFlag(kNegativeFlag);
    }

    cpu->IncreaseCycleCount(4);
}

// PLP
// increment stack pointer, pull top of stack and store in status register
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpPLP(CPU *cpu, Byte opcode){
    cpu->IncrementStackPointer();
    Byte copied_value = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());
    cpu->SetStatusRegister(copied_value);
    cpu->IncreaseCycleCount(4);
}

// RTI
// pop top of stack and store in status register
// pop top of stack and store in pc_l
// pop top of stack and store in pc_h
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpRTI(CPU *cpu, Byte opcode){
    cpu->IncrementStackPointer();
    Byte new_sr = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());
    cpu->SetStatusRegister(new_sr);

    cpu->IncrementStackPointer();
    Byte pc_l = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());
    
    cpu->IncrementStackPointer();
    Byte pc_h = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());

    cpu->SetProgramCounter((pc_h << 8) | pc_l);

    cpu->IncreaseCycleCount(6);
}

// RTS
// pop top of stack and store in pc_l
// pop top of stack and store in pc_h
// increment program counter
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpRTS(CPU *cpu, Byte opcode){

    cpu->IncrementStackPointer();
    Byte pc_l = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());
    
    cpu->IncrementStackPointer();
    Byte pc_h = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());

    cpu->SetProgramCounter((pc_h << 8) | pc_l);
    cpu->AdvanceProgramCounter();

    cpu->IncreaseCycleCount(6);
}

// JSR
// push pc_h on stack, decrement stack pointer
// push pc_l on stack, decrement stack pointer
// copy low address byte to pc_l
// copy high address byte to pc_h
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpJSR(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);

    // decrement pc to preserve last address before jump. pc will increment automatically with RTS
    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), (Byte) (cpu->GetProgramCounter()-1 >> 8));
    cpu->DecrementStackPointer();

    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), (Byte) (cpu->GetProgramCounter()-1 & 0xFF));
    cpu->DecrementStackPointer();

    cpu->SetProgramCounter(address_mode_val.value);

    cpu->IncreaseCycleCount(2);

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

// Load register X from memory
// zero flag set if loaded value is 0, otherwise cleared
// negative flag is set to 7th bit of loaded value
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpLDX(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    if (address_mode_val.is_address)
        address_mode_val.value = cpu->GetMemoryWord(address_mode_val.value);

    const auto loaded_value = address_mode_val.value;
    // set zero flag
    if (loaded_value) {
        cpu->ClearStatusRegisterFlag(kZeroFlag);
    } else {
        cpu->SetStatusRegisterFlag(kZeroFlag);
    }

    // negative flag
    if (loaded_value >> 7 == 1) {
        cpu->SetStatusRegisterFlag(kNegativeFlag);
    } else {
        cpu->ClearStatusRegisterFlag(kNegativeFlag);
    }

    cpu->SetXIndex(loaded_value);
};

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpSTX(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->WriteMemory(address_mode_val.value, cpu->GetXIndex());
};