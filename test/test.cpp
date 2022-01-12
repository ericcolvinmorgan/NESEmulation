#include <catch2/catch_test_macros.hpp>
#include "../include/emulator/emulator.h"

TEST_CASE("Properly advances frame") {
  Emulator emu;
  emu.advanceFrame();
  emu.advanceFrame();
  REQUIRE(emu.getFrame() == 2);
}

TEST_CASE("Frame error") {
  Emulator emu;
  emu.advanceFrame();
  emu.advanceFrame();
  REQUIRE(emu.getFrame() == 1);
}
