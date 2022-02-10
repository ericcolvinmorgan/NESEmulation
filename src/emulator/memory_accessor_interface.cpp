#include "../../include/emulator/memory_accessor_interface.h"

void MemoryAccessorInterface::OnMemoryWrite(Word address)
{
    for (auto subscriber : memory_change_subscribers_[address])
    {
        if (subscriber != nullptr)
        {
            subscriber->Invoke(&address);
        }
    }
}

void MemoryAccessorInterface::SubscribeMemoryChange(Word start, Word end, MemoryEventHandler *handler)
{
    if (start > end)
        std::swap(start, end);

    while (start <= end)
    {
        SubscribeMemoryChange(start, handler);
        start++;
    }
}

void MemoryAccessorInterface::SubscribeMemoryChange(Word address, MemoryEventHandler *handler)
{
    memory_change_subscribers_[address].push_back(handler);
}