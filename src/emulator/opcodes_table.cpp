#include "../../include/emulator/opcodes_table.h"

OpCodesTable::OpCodesTable()
{
    for(int i = 0; i <= 0xFF; i++)
        opcodes_[i] = {&OpCodesTable::NotImplemented, 0, 0};
}

uint8_t OpCodesTable::RunOpCode(CPU *cpu, Byte opcode) {
    ((*this).*(opcodes_[opcode].opcode_func))(cpu, opcode);
    return 0;
}