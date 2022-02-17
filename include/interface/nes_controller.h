#pragma once
#include <memory>
#include "../emulator/constants.h"
#include "../emulator/nes_cpu_memory_accessor.h"
#include "../emulator/input_interface.h"

enum class Player
{
    One,
    Two
};

class NESController
{
    private:
        bool strobing_{true};
        uint16_t controllerAddress_;
        NESCPUMemoryAccessor *memory_;
        uint8_t lastInputState_;
        std::unique_ptr<InputInterface> inputInterface_;
        std::unique_ptr<MemoryEventHandler> on_controller_write_{nullptr};
        std::unique_ptr<MemoryEventHandler> on_controller_read_{nullptr};

        void PollInput();
        void WriteMemory();

    public:
        // NESController takes ownership of InputInterface
        NESController(NESCPUMemoryAccessor *memory, InputInterface *input, Player player = Player::One);
        void AfterWrite(Byte data);
        void AfterRead();
        void PollInputIfStrobing();
        void SetInput(InputInterface *input){inputInterface_.reset(input);}
};