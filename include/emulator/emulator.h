#pragma once

class Emulator
{
private:
    int frame_ = 0;

public:
    Emulator(){};
    ~Emulator(){};
    int GetFrame() { return frame_; };
    void AdvanceFrame();
};