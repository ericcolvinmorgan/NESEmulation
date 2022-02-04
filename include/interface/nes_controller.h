#pragma once
#include "../emulator/constants.h"
#include "../emulator/nes_cpu_memory_accessor.h"
#include "../emulator/input_interface.h"

class NESController 
{
    private:
        bool strobing_{true};
        NESCPUMemoryAccessor *memory_;
        uint8_t lastInputState_;
        InputInterface *inputInterface_;
        void PollInput();

    public:
        NESController(NESCPUMemoryAccessor *memory): memory_(memory) {};
        void OnWrite(Byte data);
        void OnRead();
        void PollInputIfStrobing();
        void SetInput(InputInterface *input){inputInterface_ = input;}
};