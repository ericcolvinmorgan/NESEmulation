#pragma once
#include <functional>
#include "memory_accessor_interface.h"

class NESCPUMemoryAccessor : public MemoryAccessorInterface
{
private:
    Byte memory_[0xffff + 1] = {0};
    std::function<void(void)> OnPlayerOneRead_ = [](){};
    std::function<void(Byte)> OnPlayerOneWrite_ = [](Byte data){};

public:
    NESCPUMemoryAccessor() {}

    Byte ReadByte(uint16_t location);
    Word ReadWord(uint16_t location);
    void WriteMemory(uint16_t location, Byte data);
    void WriteMemory(uint16_t location, Word data);
    void WriteMemory(uint16_t location, const Byte *data, uint16_t length);
    // Allows a controller to write to memory without triggering its own callback
    // Everything else should use the normal WriteMemory so the controller can be properly notified
    void WritePlayerOneMemory(Byte data);
    void SetPlayerOneCallbacks(std::function<void(void)> onRead, std::function<void(Byte)> onWrite);
};