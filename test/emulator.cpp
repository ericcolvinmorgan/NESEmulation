#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/emulator.h"

TEST_CASE("Properly advances frame") {
  Emulator emu;
  emu.AdvanceFrame();
  emu.AdvanceFrame();
  REQUIRE(emu.GetFrame() == 2);
}

TEST_CASE("Zero frames") {
  Emulator emu;
  REQUIRE(emu.GetFrame() == 0);
}