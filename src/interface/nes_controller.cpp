#include "../../include/interface/nes_controller.h"

NESController::NESController(MemoryAccessorInterface *memory, InputInterface *input, Player player)
        : memory_(memory)
        , inputInterface_(input)
{
    controllerAddress_ = player == Player::One ? 0x4016 : 0x4017;
    on_controller_write_ = std::make_unique<MemoryEventHandler>([this](void *address)
                                               {
                                                   const Byte value = memory_->ReadByte(*(uint16_t *)address, true);
                                                   this->AfterWrite(value);
                                               });
    on_controller_read_ = std::make_unique<MemoryEventHandler>([this](void *address)
                                               {
                                                   this->AfterRead();
                                               });
    memory_->SubscribeMemoryChange(controllerAddress_, on_controller_write_.get());
    memory_->SubscribeMemoryRead(controllerAddress_, on_controller_read_.get());
}

void NESController::AfterWrite(Byte data)
{
    if(data & 1){
        strobing_ = true;
        PollInput();
    } else {
        strobing_ = false;
    }
    WriteMemory();
}

void NESController::AfterRead()
{
    if(!strobing_){
        lastInputState_ = lastInputState_ >> 1;
    }
    WriteMemory();
}

void NESController::PollInput()
{
    lastInputState_ = inputInterface_->GetInputState();
}

void NESController::PollInputIfStrobing()
{
    if(strobing_){
        PollInput();
    }
}

void NESController::WriteMemory()
{
    memory_->WriteMemory(controllerAddress_, static_cast<Byte>(lastInputState_ & 1), true);
}