#pragma once
#include "../emulator/controller_interface.h"

class DemoController : public ControllerInterface
{
public:
    DemoController(MemoryAccessorInterface *memory) : ControllerInterface(memory) {}

    void InitController(){};
    void WriteInput(uint16_t address);
    Byte ReadOutput(uint16_t address){ return 0; };
};