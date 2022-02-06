#pragma once
#include <functional>
#include <list>
#include <map>
#include "constants.h"

class MemoryEventHandler
{
private:
    std::function<void(void *)> callback_ = 0;

public:
    MemoryEventHandler(std::function<void(void *)> callback) { callback_ = callback; }
    ~MemoryEventHandler() {}
    void Invoke(void *data) { callback_(data); };
};

class MemoryAccessorInterface
{
protected:
    std::map<Word, std::list<MemoryEventHandler *>> memory_change_subscribers_;
    void OnMemoryWrite(Word address);

public:
    virtual ~MemoryAccessorInterface(){};
    virtual Byte ReadByte(uint16_t location) = 0;
    virtual Word ReadWord(uint16_t location) = 0;

    virtual void WriteMemory(uint16_t location, Byte data) = 0;
    virtual void WriteMemory(uint16_t location, Word data) = 0;
    virtual void WriteMemory(uint16_t location, const Byte *data, uint16_t length) = 0;

    void SubscribeMemoryChange(Word start, Word end, MemoryEventHandler *handler);
    void SubscribeMemoryChange(Word address, MemoryEventHandler *handler);
};