#pragma once
#include "constants.h"

class IMemoryAccessor
{
public:
    virtual BYTE readByte(uint16_t location) = 0;
    virtual WORD readWord(uint16_t location) = 0;
    
    virtual void writeMemory(uint16_t location, BYTE data) = 0;
    virtual void writeMemory(uint16_t location, WORD data) = 0;
    virtual void writeMemory(uint16_t location, const BYTE *data, uint16_t length) = 0;
};