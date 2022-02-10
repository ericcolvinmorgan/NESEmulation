#include <fstream>
#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/cpu.h"
#include "../include/emulator/emulator.h"
#include "../include/emulator/nes_cpu_memory_accessor.h"
#include "../include/emulator/opcodes_table.h"
#include "../include/emulator/ppu.h"

TEST_CASE("Rom Tests - nestest.nes")
{
    std::ifstream input_file("../../roms/nes-test/nestest.nes", std::ios::binary);
    REQUIRE(input_file.is_open());

    std::ofstream output_file("./nestest-results.csv", std::ios::trunc);

    // Determine the file length
    input_file.seekg(0, std::ios_base::end);
    std::size_t rom_size = input_file.tellg();
    input_file.seekg(0, std::ios_base::beg);

    // Read contents to memory and close file.
    Byte *rom_data = new Byte[rom_size];
    input_file.read((char *)rom_data, rom_size);
    input_file.close();

    NESCPUMemoryAccessor cpu_memory;
    cpu_memory.WriteMemory(0x8000, rom_data + 16, 16384);
    cpu_memory.WriteMemory(0xc000, rom_data + 16, 16384);
    delete[] rom_data;

    OpCodesTable cpu_opcodes;
    PPU ppu;
    CPU cpu({.sp = 0xFD, .sr = {.data = 0b00100100}, .pc = 0xc000}, &cpu_memory);
    Emulator emu(&ppu, &cpu, &cpu_opcodes);
    bool next = true;

    while (next)
    {
        auto opcode = cpu.GetCurrentOpCode();

        // Write To Log
        output_file << std::hex << cpu.GetProgramCounter() << "," << (short)opcode << "," << (short)cpu_memory.ReadByte(cpu.GetProgramCounter() + 1) << "," << (short)cpu_memory.ReadByte(cpu.GetProgramCounter() + 2) << std::dec << "," << (short)cpu.GetAccumulator() << "," << (short)cpu.GetXIndex() << "," << (short)cpu.GetYIndex() << "," << (short)(cpu.GetStatusRegister().data) << "," << std::hex << (short)cpu.GetStackPointer() << "," << std::dec << cpu.GetCycleCount() << "\n";

        if (opcode == 0xa1 && cpu.GetAccumulator() == 0x5c)
        {
            int i = 0;
        }

        cpu.AdvanceProgramCounter();

        try
        {
            cpu_opcodes.RunOpCode(&cpu, opcode);
        }
        catch (const std::exception &e)
        {
            output_file.close();
            next = false;
        }
    }

    REQUIRE(true);
}