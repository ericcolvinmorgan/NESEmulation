#pragma once
#include "constants.h"

class MemoryAccessorInterface
{
public:
    virtual Byte ReadByte(uint16_t location) = 0;
    virtual Word ReadWord(uint16_t location) = 0;
    
    virtual void WriteMemory(uint16_t location, Byte data) = 0;
    virtual void WriteMemory(uint16_t location, Word data) = 0;
    virtual void WriteMemory(uint16_t location, const Byte *data, uint16_t length) = 0;
};