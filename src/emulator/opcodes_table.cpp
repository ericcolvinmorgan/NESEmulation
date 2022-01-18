#include "../../include/emulator/opcodes_table.h"

OpCodesTable::OpCodesTable()
{
    for(int i = 0; i <= 0xFF; i++)
        _opcodes[i] = {&OpCodesTable::notImplemented, 0, 0};
}

uint8_t OpCodesTable::runOpCode(CPU *cpu, BYTE opcode) {
    ((*this).*(_opcodes[opcode].opcodeFunc))(cpu, opcode);
    return 0;
}