#include "../../include/emulator/ppu.h"
#include "../../include/emulator/constants.h"

PPU::PPU(NESPPUMemoryAccessor *memory)
{
    memory_ = memory;
    reg_ctrl_.data = 0;
}

PPU::PPU(NESPPUMemoryAccessor *memory, Byte ctrl_flags)
{
    memory_ = memory;
    reg_ctrl_.data = ctrl_flags;
}

void PPU::WriteToAddrReg(Byte data)
{
    if (latch_ == 0) // first write in 2-write sequence
    {
        temp_address_ = (data << 8);
        latch_ = 1;
    }
    else
    {
        temp_address_ |= data;
        vram_address_ = temp_address_ & 0x3fff; // mirror down if addr > 3fff
        latch_ = 0;
    }
}

Byte PPU::ReadFromDataReg()
{
    Byte data = interal_buffer_;
    interal_buffer_ = memory_->ReadByte(vram_address_);

    if (vram_address_ >= 0x3f00) // palette memory is not delayed
    { 
        data = memory_->ReadByte(vram_address_);
    }
    
    if (reg_ctrl_.flags.increment)
    {
        vram_address_ += 32;
    }
    else
    {
        vram_address_++;
    }
    return data;
}