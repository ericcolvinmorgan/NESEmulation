#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/cpu.h"
#include "../include/emulator/emulator.h"
#include "../include/emulator/raw_memory_accessor.h"
#include "../include/emulator/opcodes_table.h"

TEST_CASE("Emulator - Zero frames") {
  RawMemoryAccessor memory;
  OpCodesTable cpu_opcodes;
  CPU cpu(&memory);
  Emulator emu(&cpu, &cpu_opcodes);
  REQUIRE(emu.GetFrame() == 0);
}