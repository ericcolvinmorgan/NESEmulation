#include "../../include/emulator/ppu.h"
#include "../../include/emulator/constants.h"

PPU::PPU(MemoryAccessorInterface *ppu_memory, MemoryAccessorInterface *cpu_memory)
{
    ppu_memory_ = ppu_memory;
    cpu_memory_ = cpu_memory;

    on_ppuctrl_write_ = new MemoryEventHandler([this](void *address)
                                               { this->HandlePPUCTRLWrite(address); });
    cpu_memory_->SubscribeMemoryChange(kPPUCTRL, on_ppuctrl_write_);

    on_ppumask_write_ = new MemoryEventHandler([this](void *address)
                                               { this->HandlePPUMASKWrite(address); });
    cpu_memory_->SubscribeMemoryChange(kPPUMASK, on_ppumask_write_);

    on_oamaddr_write_ = new MemoryEventHandler([this](void *address)
                                               { this->HandleOAMADDRWrite(address); });
    cpu_memory_->SubscribeMemoryChange(kOAMADDR, on_oamaddr_write_);

    on_oamdata_write_ = new MemoryEventHandler([this](void *address)
                                               { this->HandleOAMDATAWrite(address); });
    cpu_memory_->SubscribeMemoryChange(kOAMDATA, on_oamdata_write_);

    on_ppuscroll_write_ = new MemoryEventHandler([this](void *address)
                                                 { this->HandlePPUSCROLLWrite(address); });
    cpu_memory_->SubscribeMemoryChange(kPPUSCROLL, on_ppuscroll_write_);

    on_ppuaddr_write_ = new MemoryEventHandler([this](void *address)
                                               { this->HandlePPUADDRWrite(address); });
    cpu_memory_->SubscribeMemoryChange(kPPUADDR, on_ppuaddr_write_);

    on_ppudata_write_ = new MemoryEventHandler([this](void *address)
                                               { this->HandlePPUDATAWrite(address); });
    cpu_memory_->SubscribeMemoryChange(kPPUDATA, on_ppudata_write_);

    on_oamdma_write_ = new MemoryEventHandler([this](void *address)
                                               { this->HandleOAMDMAWrite(address); });
    cpu_memory_->SubscribeMemoryChange(kOAMDMA, on_oamdma_write_);
}

void PPU::HandlePPUCTRLWrite(void *address)
{
    Byte data = cpu_memory_->ReadByte(*(uint16_t *)address);
    reg_ctrl_.data = data;
}

void PPU::HandlePPUMASKWrite(void *address)
{
    Byte data = cpu_memory_->ReadByte(*(uint16_t *)address);
    reg_mask_.data = data;
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
    ppu_memory_->WriteMemory(vram_address_, data);
    IncrementPPUAddr();
}

// a write to 0x4014 indicates DMA transfer of 256 bytes from CPU memory
// starting at operand %XX
void PPU::HandleOAMDMAWrite(void *address)
{
    Byte data = cpu_memory_->ReadByte(*(uint16_t *)address);
    dma_addr_page = data;
    is_dma_transferring = true;
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

    IncrementPPUAddr();
    return data;
}

void PPU::RunEvents()
{
    switch (cycle_scanline_)
    {
    case 0 ... 239: // Visible Frame Lines
    {

        switch (cycle_pixel_)
        {
        case 1 ... 64: 
        {
            std::fill(secondary_oam_, secondary_oam_ + 32, (Byte)0xFF); // clear sprites
        }
        case 65 ... 256:
        {
            // sprite evaluation
            int n = 0;
            while (n < 64 && num_sprites < 9)
            {
                Byte y_pos = oam_[n * 4];
                if (cycle_scanline_ - y_pos >= 0) // top position in range
                {
                    if (num_sprites < 8)
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            secondary_oam_[(num_sprites * 4) + i] = oam_[(n * 4) + i];
                        }
                        num_sprites++;
                    }
                }
                n++;
            }

            if (num_sprites == 9)
            {
                reg_status_.flags.overflow = 1;
            }
            
        }

        case 321 ... 336:
        {
            if ((cycle_pixel_ % 8) == 1) // Nametable Fetch
            {
            }
            else if ((cycle_pixel_ % 8) == 3) // Attribute table Fetch
            {
            }
            else if ((cycle_pixel_ % 8) == 5) // Low BG Tile Fetch
            {
            }
            else if ((cycle_pixel_ % 8) == 7) // High BG Tile Fetch
            {
            }
        }
        break;
        }
    }
    break;

    case 240 ... 260: // Post-Render Frame Lines
    {
        // Set VBlank
        if (cycle_scanline_ == 241 && cycle_pixel_ == 1)
        {
            Byte status = cpu_memory_->ReadByte(kPPUSTATUS);
            cpu_memory_->WriteMemory(kPPUSTATUS, (Byte)(status | 0b10000000), true);
            if (reg_ctrl_.flags.nmi)
                nmi_requested_ = true;
        }

        if (is_dma_transferring){
            for(int i = 0; i < 256; i++){
                dma_data = cpu_memory_->ReadByte((dma_addr_page << 8) | oam_address_);
                oam_[oam_address_] = dma_data;
                oam_address_++;
            }
            
            is_dma_transferring = false;
            oam_address_ = 0;
        }
        
    }
    break;

    case 261: // Pre-Render Frame Line
    {
        // End VBlank
        if (cycle_pixel_ == 1)
        {
            Byte status = cpu_memory_->ReadByte(kPPUSTATUS);
            cpu_memory_->WriteMemory(kPPUSTATUS, (Byte)(status | 0b00000000), true);

            reg_status_.flags.overflow = 0;
        }

        switch (cycle_pixel_)
        {
        case 1 ... 256:
        case 321 ... 336:
        {
            if ((cycle_pixel_ % 8) == 1) // Nametable Fetch
            {
            }
            else if ((cycle_pixel_ % 8) == 3) // Attribute table Fetch
            {
            }
            else if ((cycle_pixel_ % 8) == 5) // Low BG Tile Fetch
            {
            }
            else if ((cycle_pixel_ % 8) == 7) // High BG Tile Fetch
            {
            }
        }
        break;

        // TO START WE WILL JUST LOAD EVERYTHING AT THE END OF THE PRE-RENDER LINE.  THIS IS NOT PROPER AND SHOULD BE REPLACED.
        case 340:
        {
            FillScreenBuffer();
        }
        break;
        }
    }
    break;
    }
}

void PPU::IncrementPPUAddr()
{
    if (reg_ctrl_.flags.increment)
    {
        vram_address_ += 32;
    }
    else
    {
        vram_address_++;
    }
}

void PPU::RunCycle()
{
    RunEvents();

    cycle_pixel_ = ++cycle_pixel_ % 341;
    if (cycle_pixel_ == 0)
        cycle_scanline_ = ++cycle_scanline_ % 262;
}

void PPU::FillScreenBuffer()
{
    // Current Nametable
    uint16_t base_nametable = 0x2000;
    uint16_t base_attribute = 0x23c0;
    if (reg_ctrl_.flags.nametable == 1)
    {
        base_nametable = 0x2400;
        base_attribute = 0x27c0;
    }
    else if (reg_ctrl_.flags.nametable == 2)
    {
        base_nametable = 0x2800;
        base_attribute = 0x2bc0;
    }
    else if (reg_ctrl_.flags.nametable == 3)
    {
        base_nametable = 0x2c00;
        base_attribute = 0x2fc0;
    }

    for (int w = 0; w < 32; w++)
    {
        for (int h = 0; h < 30; h++)
        {
            // Fetch Nametable Tile
            Byte nt_tile = ppu_memory_->ReadByte(base_nametable + ((h * 32) + w));
            int16_t nt_index = nt_tile << 4;

            int16_t addr = base_attribute + ((h / 4) * 8) + (w / 4);
            Byte palette_tile = ppu_memory_->ReadByte(base_attribute + ((h / 4) * 8) + (w / 4));
            
            // Each tile represents an 8X8 tile of pixels
            for (int p_h = 0; p_h < 8; p_h++)
            {
                // Get Nametable Bytes
                Byte top_byte = ppu_memory_->ReadByte(nt_index + p_h + (0x1000 * reg_ctrl_.flags.background_addr));
                Byte bottom_byte = ppu_memory_->ReadByte(nt_index + p_h + 8 + (0x1000 * reg_ctrl_.flags.background_addr));
                for (int p_w = 0; p_w < 8; p_w++)
                {
                    int base_index = ((h * 32 * (8 * 8)) + (w * 8) + (p_h * 32 * 8) + p_w);
                    
                    Byte sub_palette = palette_tile;
                    if((w % 4) >= 2)
                        sub_palette = sub_palette >> 2;

                    if((h % 4) >= 2)
                        sub_palette = sub_palette >> 4;
                        
                    sub_palette &= 0b00000011;
                    uint8_t palette_index = ((top_byte >> (7 - p_w)) & 0b00000001) | (((bottom_byte >> (7 - p_w)) & 0b00000001) << 1);
                    Byte color = ppu_memory_->ReadByte(0x3f00 + (sub_palette * 4) + palette_index);
                    screen_buffer_[base_index] = color;
                }
            }
        }
    }
}