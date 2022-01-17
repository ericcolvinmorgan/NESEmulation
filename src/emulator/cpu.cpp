#include "../../include/emulator/cpu.h"
#include "../../include/emulator/constants.h"

void CPU::advanceProgramCounter()
{
    _pc++;
}

void CPU::reset()
{
    _pc++;
}