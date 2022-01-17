#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/raw_memory_accessor.h"

TEST_CASE("RawMemoryAccessor - Successfully updates BYTE data")
{
  RawMemoryAccessor memory;
  memory.writeMemory(0x0000, (BYTE)0x01);
  REQUIRE(memory.readByte(0x00) == 1);
  REQUIRE(memory.readByte(0x01) == 0);
  memory.writeMemory(0xFFFF, (BYTE)0xFF);
  REQUIRE(memory.readByte(0xFFFF) == 0xFF);
}

TEST_CASE("RawMemoryAccessor - Successfully updates WORD data")
{
  BYTE bytes[] = {0x2c,	0x01};
  WORD word = 300;
  RawMemoryAccessor memory;
  memory.writeMemory(0x0100, word);

  REQUIRE(memory.readWord(0x0100) == word);
  for (int i = 0; i < 2; i++)
  {
    REQUIRE(memory.readByte(0x0100 + i) == bytes[i]);
  }
}

TEST_CASE("RawMemoryAccessor - Successfully reads WORD data")
{
  BYTE bytes[] = {0x2c,	0x01,	0x90,	0x01,	0xf4,	0x01};
  WORD words[] = {300, 400, 500};
  RawMemoryAccessor memory;
  memory.writeMemory(0x0100, bytes, 6);

  for (int i = 0; i < 3; i++)
  {
    REQUIRE(memory.readWord(0x0100 + (i * 2)) == words[i]);
  }
}

TEST_CASE("RawMemoryAccessor - Successfully updates array of data")
{
  BYTE testData[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  RawMemoryAccessor memory;
  memory.writeMemory(0x0100, testData, 10);
  for (int i = 0; i < 10; i++)
  {
    REQUIRE(memory.readByte(0x0100 + i) == testData[i]);
  }
}