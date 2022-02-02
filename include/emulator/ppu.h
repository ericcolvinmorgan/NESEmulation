#pragma once

#include "constants.h"

class PPU {
    private:
        Byte pattern_table_[8 * 1024]; // static ch-rom
        Byte name_table_[4 * 1024]; // dynamic vram
        Byte palette_[32]; // color palette
        Byte oam_[64]; // object attribute memory

    
        struct PPUCtrl
        {
            union
            {
                Byte data; // 0bVPHBSINN
                struct {
                    Byte N0 : 1; //
                    Byte N1 : 1; // ^ base nametable address
                    Byte I  : 1; // vram increment per cpu read/write of PPUDATA
                    Byte S  : 1; // sprite pattern table address
                    Byte B  : 1; // background pattern table address
                    Byte H  : 1; // sprite size
                    Byte P  : 1; // PPU master/slave select
                    Byte V  : 1; // generate NMI at start of vblank
                } flags;
            };
        } reg_ctrl_; // PPUCTRL $2000 > Write

        struct PPUMask
        {
            union
            {
                Byte data; // 0bBGRsBMmG
                struct {
                    Byte G : 1; // produce greyscale display
                    Byte bg_left : 1; // show background in leftmost 8 pixels of screen
                    Byte spr_left  : 1; // show sprites in leftmost 8 pixels of screen
                    Byte bg  : 1; // show background
                    Byte spr  : 1; // show sprite
                    Byte R  : 1; // emphasize red
                    Byte G  : 1; // emphasize green
                    Byte B  : 1; // emphasize blue
                } flags;
            };
        } reg_mask_; // PPUMASK $2001 > Write

        struct PPUStatus
        {
            union
            {
                Byte data; // 0bVSO-----
                struct {
                    Byte : 5; // not used
                    Byte overflow : 1; // sprite overflow
                    Byte S  : 1; // sprite 0 hit
                    Byte V  : 1; // vblank has started
                } flags;
            };
        } reg_status_; // PPUMASK $2001 > Write

        Byte reg_oamaddr_;
        Byte reg_oamdata_;
        Byte reg_scroll_;
        Byte reg_addr_;
        Byte reg_data_;





    public:
        PPU();
};