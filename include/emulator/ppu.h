#pragma once

#include "constants.h"
#include "nes_ppu_memory_accessor.h"
#include "nes_cpu_memory_accessor.h"
#include "memory_accessor_interface.h"

class PPU
{
private:
    const uint16_t kPPUCTRL = 0x2000;
    const uint16_t kPPUMASK = 0x2001;
    const uint16_t kPPUSTATUS = 0x2002;
    const uint16_t kOAMADDR = 0x2003;
    const uint16_t kOAMDATA = 0x2004;
    const uint16_t kPPUSCROLL = 0x2005;
    const uint16_t kPPUADDR = 0x2006;
    const uint16_t kPPUDATA = 0x2007;

    Byte pattern_table_[8 * 1024] = {0}; // static ch-rom
    Byte name_table_[4 * 1024] = {0};    // dynamic vram
    Byte palette_[32] = {0};             // color palette
    Byte oam_[64] = {0};                 // object attribute memory

    Byte screen_buffer_[(32 * 8) * (30 * 8)] = {0};

    struct PPUCtrl
    {
        union
        {
            Byte data; // 0bVPHBSINN
            struct
            {
                Byte nametable : 2;          // ^ base nametable address
                Byte increment : 1;           // vram increment per cpu read/write of PPUDATA
                                              // if 0, increment 1 else increment 32
                Byte sprite_addr : 1;         // sprite pattern table address
                Byte background_addr : 1;     // background pattern table address
                Byte sprite_size : 1;         // sprite size
                Byte master_slave_select : 1; // PPU master/slave select
                Byte nmi : 1;                 // generate NMI at start of vblank
            } flags;
        };
    } reg_ctrl_; // PPUCTRL $2000 > Write

    struct PPUMask
    {
        union
        {
            Byte data; // 0bBGRsBMmG
            struct
            {
                Byte greyscale : 1;       // produce greyscale display
                Byte background_left : 1; // show background in leftmost 8 pixels of screen
                Byte sprite_left : 1;     // show sprites in leftmost 8 pixels of screen
                Byte background : 1;      // show background
                Byte sprite : 1;          // show sprite
                Byte red : 1;             // emphasize red
                Byte green : 1;           // emphasize green
                Byte blue : 1;            // emphasize blue
            } flags;
        };
    } reg_mask_; // PPUMASK $2001 > Write

    struct PPUStatus
    {
        union
        {
            Byte data; // 0bVSO-----
            struct
            {
                Byte : 5;                // not used
                Byte overflow : 1;       // sprite overflow
                Byte sprite_hit : 1;     // sprite 0 hit
                Byte vblank_started : 1; // vblank has started
            } flags;
        };
    } reg_status_; // PPUMASK $2002 > Write

    uint16_t temp_address_ = 0; // used with ppuaddr
    uint16_t vram_address_ = 0;
    Byte latch_ = 0;          // indicates firts or second write in ppuaddr
    Byte interal_buffer_ = 0; // holds discarded value on first read from ppudata

    int16_t cycle_pixel_ = 0;
    int16_t cycle_scanline_ = 0;
    bool nmi_requested_ = false;

    MemoryAccessorInterface *ppu_memory_;
    MemoryAccessorInterface *cpu_memory_;

    MemoryEventHandler *on_ppuctrl_write_ = nullptr;
    MemoryEventHandler *on_ppumask_write_ = nullptr;
    MemoryEventHandler *on_oamaddr_write_ = nullptr;
    MemoryEventHandler *on_oamdata_write_ = nullptr;
    MemoryEventHandler *on_ppuscroll_write_ = nullptr;
    MemoryEventHandler *on_ppuaddr_write_ = nullptr;
    MemoryEventHandler *on_ppudata_write_ = nullptr;

    // PPU Register Write Events
    void HandlePPUCTRLWrite(void *address);
    void HandlePPUMASKWrite(void *address);
    void HandleOAMADDRWrite(void *address);
    void HandleOAMDATAWrite(void *address);
    void HandlePPUSCROLLWrite(void *address);
    void HandlePPUADDRWrite(void *address);
    void HandlePPUDATAWrite(void *address);
    void RunEvents();

    void IncrementPPUAddr();

    // This is a temporary function for initial testing, we will need to fill the buffer incrementally as we further implement cycles. 
    void FillScreenBuffer();

public:
    PPU(){};
    PPU(MemoryAccessorInterface *ppu_memory, MemoryAccessorInterface *cpu_memory);

    Byte HandlePPUDATARead();

    // for testing
    Word getVram() { return vram_address_; };
    void setPPUCTRLData(Byte data) { reg_ctrl_.data = data; }
    void RunCycle();

    bool NMIRequested() { return nmi_requested_; };
    void ClearNMIRequest() { nmi_requested_ = false; };
    Byte* GetScreenBuffer() { return screen_buffer_; }
};