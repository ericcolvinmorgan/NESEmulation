#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/nes_cpu_memory_accessor.h"

TEST_CASE("NESCPUMemoryAccessor - Successfully updates BYTE data")
{
  NESCPUMemoryAccessor memory;
  memory.WriteMemory(0x0000, (Byte)0x01);
  REQUIRE(memory.ReadByte(0x00) == 1);
  REQUIRE(memory.ReadByte(0x01) == 0);
  memory.WriteMemory(0xFFFF, (Byte)0xFF);
  REQUIRE(memory.ReadByte(0xFFFF) == 0xFF);
}

TEST_CASE("NESCPUMemoryAccessor - Successfully updates WORD data")
{
  Byte bytes[] = {0x2c,	0x01};
  Word word = 300;
  NESCPUMemoryAccessor memory;
  memory.WriteMemory(0x0100, word);

  REQUIRE(memory.ReadWord(0x0100) == word);
  for (int i = 0; i < 2; i++)
  {
    REQUIRE(memory.ReadByte(0x0100 + i) == bytes[i]);
  }
}

TEST_CASE("NESCPUMemoryAccessor - Successfully reads WORD data")
{
  Byte bytes[] = {0x2c,	0x01,	0x90,	0x01,	0xf4,	0x01};
  Word words[] = {300, 400, 500};
  NESCPUMemoryAccessor memory;
  memory.WriteMemory(0x0100, bytes, 6);

  for (int i = 0; i < 3; i++)
  {
    REQUIRE(memory.ReadWord(0x0100 + (i * 2)) == words[i]);
  }
}

TEST_CASE("NESCPUMemoryAccessor - Successfully updates array of data")
{
  Byte testData[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  NESCPUMemoryAccessor memory;
  memory.WriteMemory(0x0100, testData, 10);
  for (int i = 0; i < 10; i++)
  {
    REQUIRE(memory.ReadByte(0x0100 + i) == testData[i]);
  }
}