#include "../../include/interface/nes_controller.h"

void NESController::OnWrite(Byte data)
{
    if(data & 1){
        strobing_ = true;
        PollInput();
    } else {
        strobing_ = false;
    }
    memory_->WritePlayerOneMemory(lastInputState_ & 1);
}

void NESController::OnRead()
{
    if(!strobing_){
        lastInputState_ = lastInputState_ >> 1;
    }
    memory_->WritePlayerOneMemory(lastInputState_ & 1);
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