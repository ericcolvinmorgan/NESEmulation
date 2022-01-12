class Emulator
{
private:
    int _frame = 0;

public:
    Emulator(){};
    ~Emulator(){};
    int getFrame() { return _frame; };
    void advanceFrame();
};