#pragma once
#include "../emulator/controller_interface.h"

class DemoController : public ControllerInterface
{
public:
    DemoController(MemoryAccessorInterface *memory) : ControllerInterface(memory) {}

    void InitController(){};
    void WriteInput(Byte address);
    void ReadOutput(Byte address){};
};