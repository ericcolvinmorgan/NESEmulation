#include "../../include/emulator/ppu.h"
#include "../../include/emulator/constants.h"

PPU::PPU(NESPPUMemoryAccessor *ppu_memory, NESCPUMemoryAccessor *cpu_memory)
{
    ppu_memory_ = ppu_memory;
    cpu_memory_ = cpu_memory;

    std::function<void(void *)> f = [this](void *address)
    { this->HandleAddressRegisterWrite(address); };
    on_address_register_write_ = new MemoryEventHandler(f);
    cpu_memory_->SubscribeMemoryChange(0x2006, on_address_register_write_);
}

void PPU::HandleAddressRegisterWrite(void *address)
{
    Byte data = cpu_memory_->ReadByte(0x2006);
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
    interal_buffer_ = ppu_memory_->ReadByte(vram_address_);

    if (vram_address_ >= 0x3f00) // palette memory is not delayed
    {
        data = ppu_memory_->ReadByte(vram_address_);
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