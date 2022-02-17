#pragma once
#include "../emulator/input_interface.h"

class KeyboardInterface : public InputInterface
{   
public:
    KeyboardInterface() = default;
    uint8_t GetInputState() const override;
};