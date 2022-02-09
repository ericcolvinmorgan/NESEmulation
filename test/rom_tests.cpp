#include <fstream>
#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/cpu.h"
#include "../include/emulator/emulator.h"
#include "../include/emulator/nes_cpu_memory_accessor.h"
#include "../include/emulator/opcodes_table.h"

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

    NESCPUMemoryAccessor memory;
    memory.WriteMemory(0x8000, rom_data + 16, 16384);
    memory.WriteMemory(0xc000, rom_data + 16, 16384);
    delete[] rom_data;

    OpCodesTable cpu_opcodes;
    CPU cpu({.sp = 0xFD, .sr = {.data = 0b00100100}, .pc = 0xc000}, &memory);
    cpu.IncreaseCycleCount(7); // // https://www.pagetable.com/?p=410
    Emulator emu(&cpu, &cpu_opcodes);
    bool next = true;

    while (next)
    {
        auto opcode = cpu.GetCurrentOpCode();

        // Write To Log
        output_file << std::hex << cpu.GetProgramCounter() << "," << (short)opcode << "," << (short)memory.ReadByte(cpu.GetProgramCounter() + 1) << "," << (short)memory.ReadByte(cpu.GetProgramCounter() + 2) << std::dec << "," << (short)cpu.GetAccumulator() << "," << (short)cpu.GetXIndex() << "," << (short)cpu.GetYIndex() << "," << (short)(cpu.GetStatusRegister().data) << "," << std::hex << (short)cpu.GetStackPointer() << "," << std::dec << cpu.GetCycleCount() << "\n";

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