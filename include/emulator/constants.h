#pragma once

#include <stdint.h>

typedef uint8_t Byte;
typedef uint16_t Word;

const Word kReset = 0xFFFC;
const Byte kCarryFlag = 1 << 7;
const Byte kZeroFlag = 1 << 6;
const Byte kInterruptFlag = 1 << 5;
const Byte kDecimalFlag = 1 << 4;
const Byte kBreakFlag = 1 << 3;
const Byte kOverflowFlag = 1 << 1;
const Byte kNegativeFlag = 1;
