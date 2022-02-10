#pragma once
#include <functional>
#include "memory_accessor_interface.h"

class NESCPUMemoryAccessor : public MemoryAccessorInterface
{
private:
    Byte memory_[0xffff + 1] = {0};
    std::function<void(void)> AfterPlayerOneRead_ = [](){};
    std::function<void(Byte)> AfterPlayerOneWrite_ = [](Byte data){};

public:
    NESCPUMemoryAccessor() {}
    ~NESCPUMemoryAccessor() {}

    Byte ReadByte(uint16_t location);
    Word ReadWord(uint16_t location);
    void WriteMemory(uint16_t location, Byte data, bool suppress_event = false);
    void WriteMemory(uint16_t location, Word data, bool suppress_event = false);
    void WriteMemory(uint16_t location, const Byte *data, uint16_t length);
    // Allows a controller to write to memory without triggering its own callback
    // Everything else should use the normal WriteMemory so the controller can be properly notified
    void WritePlayerOneMemory(Byte data);
    void SetPlayerOneCallbacks(std::function<void(void)> afterRead, std::function<void(Byte)> afterWrite);
};