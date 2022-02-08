#include "../../include/emulator/ppu.h"
#include "../../include/emulator/constants.h"

PPU::PPU(MemoryAccessorInterface *ppu_memory, MemoryAccessorInterface *cpu_memory)
{
    ppu_memory_ = ppu_memory;
    cpu_memory_ = cpu_memory;

    // Set VBlank to true - this shouldn't actually be done, but is being done so we can trigger CPU to start populating nametables.
    cpu_memory_->WriteMemory(0x2002, (Byte)0b10000000);

    on_ppuctrl_write_ = new MemoryEventHandler([this](void *address)
                                               { this->HandlePPUCTRLWrite(address); });
    cpu_memory_->SubscribeMemoryChange(0x2000, on_ppuctrl_write_);

    on_ppumask_write_ = new MemoryEventHandler([this](void *address)
                                               { this->HandlePPUMASKWrite(address); });
    cpu_memory_->SubscribeMemoryChange(0x2001, on_ppumask_write_);

    on_oamaddr_write_ = new MemoryEventHandler([this](void *address)
                                               { this->HandleOAMADDRWrite(address); });
    cpu_memory_->SubscribeMemoryChange(0x2003, on_oamaddr_write_);

    on_oamdata_write_ = new MemoryEventHandler([this](void *address)
                                               { this->HandleOAMDATAWrite(address); });
    cpu_memory_->SubscribeMemoryChange(0x2004, on_oamdata_write_);

    on_ppuscroll_write_ = new MemoryEventHandler([this](void *address)
                                                 { this->HandlePPUSCROLLWrite(address); });
    cpu_memory_->SubscribeMemoryChange(0x2005, on_ppuscroll_write_);

    on_ppuaddr_write_ = new MemoryEventHandler([this](void *address)
                                               { this->HandlePPUADDRWrite(address); });
    cpu_memory_->SubscribeMemoryChange(0x2006, on_ppuaddr_write_);

    on_ppudata_write_ = new MemoryEventHandler([this](void *address)
                                               { this->HandlePPUDATAWrite(address); });
    cpu_memory_->SubscribeMemoryChange(0x2007, on_ppudata_write_);
}

void PPU::HandlePPUCTRLWrite(void *address)
{
    Byte data = cpu_memory_->ReadByte(*(uint16_t *)address);
    reg_ctrl_.data = data;
}

void PPU::HandlePPUMASKWrite(void *address)
{
    Byte data = cpu_memory_->ReadByte(*(uint16_t *)address);
}

void PPU::HandleOAMADDRWrite(void *address)
{
    Byte data = cpu_memory_->ReadByte(*(uint16_t *)address);
}

void PPU::HandleOAMDATAWrite(void *address)
{
    Byte data = cpu_memory_->ReadByte(*(uint16_t *)address);
}

void PPU::HandlePPUSCROLLWrite(void *address)
{
    Byte data = cpu_memory_->ReadByte(*(uint16_t *)address);
}

void PPU::HandlePPUADDRWrite(void *address)
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

void PPU::HandlePPUDATAWrite(void *address)
{
    Byte data = cpu_memory_->ReadByte(*(uint16_t *)address);
}

// still need to implement as handler
Byte PPU::HandlePPUDATARead()
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