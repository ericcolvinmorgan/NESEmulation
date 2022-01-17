#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/cpu.h"
#include "../include/emulator/raw_memory_accessor.h"

// TEST_CASE("CPU - Successfully loads first OP")
// {
//   BYTE testData[] = {0xa9, 0x01, 0x8d, 0x00, 0x02, 0xa9, 0x05, 0x8d, 0x01, 0x02, 0xa9, 0x08, 0x8d, 0x02, 0x02};
//   RawMemoryAccessor memory;
//   memory.writeMemory(0x0100, testData, 15);
//   CPU cpu(&memory);
//   cpu.reset();
//   cpu.advanceProgramCounter();
//   cpu.advanceProgramCounter();
//   REQUIRE(cpu.getProgramCounter() == 2);
// }