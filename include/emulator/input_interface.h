#pragma once
#include <stdint.h>

class InputInterface
{   public:
        InputInterface() = default;
        virtual uint8_t GetInputState() const = 0;
};