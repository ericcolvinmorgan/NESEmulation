#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/cpu.h"

TEST_CASE("Properly advances counter") {
  CPU cpu;
  cpu.advanceProgramCounter();
  cpu.advanceProgramCounter();
  REQUIRE(cpu.getProgramCounter() == 2);
}

TEST_CASE("Zero counter") {
  CPU cpu;
  REQUIRE(cpu.getProgramCounter() == 0);
}