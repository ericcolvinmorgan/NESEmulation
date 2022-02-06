
#include <stdexcept>
#include "../../include/emulator/opcodes_table.h"

/*
TODO - OPEN ITEMS:

1) I'm a little unclear how to handle wrap around when we exceed FFFF.
https://softpixel.com/~cwright/sianse/docs/65816NFO.HTM
For absolute indexed, this resource seems to indicate we'd wrap around
into the zero page, though I know I've seen other resources that seem
to extend past FFFF and not wrap.

*/

OpCodesTable::OpCodesTable()
{
    opcodes_[0x00] = &OpCodesTable::OpBRK<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x01] = &OpCodesTable::OpORA<&OpCodesTable::AddressingModeIndirectX>;
    opcodes_[0x02] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x03] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x04] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x05] = &OpCodesTable::OpORA<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x06] = &OpCodesTable::OpASL<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x07] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x08] = &OpCodesTable::OpPHP<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x09] = &OpCodesTable::OpORA<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0x0a] = &OpCodesTable::OpASL<&OpCodesTable::AddressingModeAccumulator>;
    opcodes_[0x0b] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x0c] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x0d] = &OpCodesTable::OpORA<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x0e] = &OpCodesTable::OpASL<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x0f] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x10] = &OpCodesTable::OpBPL<&OpCodesTable::AddressingModeRelative>;
    opcodes_[0x11] = &OpCodesTable::OpORA<&OpCodesTable::AddressingModeIndirectY>;
    opcodes_[0x12] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x13] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x14] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x15] = &OpCodesTable::OpORA<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x16] = &OpCodesTable::OpASL<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x17] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x18] = &OpCodesTable::OpCLC<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x19] = &OpCodesTable::OpORA<&OpCodesTable::AddressingModeAbsoluteY>;
    opcodes_[0x1a] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x1b] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x1c] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0x1d] = &OpCodesTable::OpORA<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0x1e] = &OpCodesTable::OpASL<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0x1f] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x20] = &OpCodesTable::OpJSR<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x21] = &OpCodesTable::OpAND<&OpCodesTable::AddressingModeIndirectX>;
    opcodes_[0x22] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x23] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x24] = &OpCodesTable::OpBIT<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x25] = &OpCodesTable::OpAND<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x26] = &OpCodesTable::OpROL<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x27] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x28] = &OpCodesTable::OpPLP<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x29] = &OpCodesTable::OpAND<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0x2a] = &OpCodesTable::OpROL<&OpCodesTable::AddressingModeAccumulator>;
    opcodes_[0x2b] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x2C] = &OpCodesTable::OpBIT<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x2d] = &OpCodesTable::OpAND<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x2e] = &OpCodesTable::OpROL<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x2f] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x30] = &OpCodesTable::OpBMI<&OpCodesTable::AddressingModeRelative>;
    opcodes_[0x31] = &OpCodesTable::OpAND<&OpCodesTable::AddressingModeIndirectY>;
    opcodes_[0x32] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x33] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x34] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x35] = &OpCodesTable::OpAND<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x36] = &OpCodesTable::OpROL<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x37] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x38] = &OpCodesTable::OpSEC<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x39] = &OpCodesTable::OpAND<&OpCodesTable::AddressingModeAbsoluteY>;
    opcodes_[0x3a] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x3b] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x3c] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0x3d] = &OpCodesTable::OpAND<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0x3e] = &OpCodesTable::OpROL<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0x3f] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x40] = &OpCodesTable::OpRTI<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x41] = &OpCodesTable::OpEOR<&OpCodesTable::AddressingModeIndirectX>;
    opcodes_[0x42] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x43] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x44] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x45] = &OpCodesTable::OpEOR<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x46] = &OpCodesTable::OpLSR<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x47] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x48] = &OpCodesTable::OpPHA<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x49] = &OpCodesTable::OpEOR<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0x4a] = &OpCodesTable::OpLSR<&OpCodesTable::AddressingModeAccumulator>;
    opcodes_[0x4b] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x4c] = &OpCodesTable::OpJMP<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x4d] = &OpCodesTable::OpEOR<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x4e] = &OpCodesTable::OpLSR<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x4f] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x50] = &OpCodesTable::OpBVC<&OpCodesTable::AddressingModeRelative>;
    opcodes_[0x51] = &OpCodesTable::OpEOR<&OpCodesTable::AddressingModeIndirectY>;
    opcodes_[0x52] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x53] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x54] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x55] = &OpCodesTable::OpEOR<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x56] = &OpCodesTable::OpLSR<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x57] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x58] = &OpCodesTable::OpCLI<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x59] = &OpCodesTable::OpEOR<&OpCodesTable::AddressingModeAbsoluteY>;
    opcodes_[0x5a] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x5b] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x5c] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0x5d] = &OpCodesTable::OpEOR<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0x5e] = &OpCodesTable::OpLSR<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0x5f] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x60] = &OpCodesTable::OpRTS<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x61] = &OpCodesTable::OpADC<&OpCodesTable::AddressingModeIndirectX>;
    opcodes_[0x62] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x63] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x64] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x65] = &OpCodesTable::OpADC<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x66] = &OpCodesTable::OpROR<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x67] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x68] = &OpCodesTable::OpPLA<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x69] = &OpCodesTable::OpADC<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0x6a] = &OpCodesTable::OpROR<&OpCodesTable::AddressingModeAccumulator>;
    opcodes_[0x6b] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x6c] = &OpCodesTable::OpJMP<&OpCodesTable::AddressingModeAbsoluteIndirect>;
    opcodes_[0x6d] = &OpCodesTable::OpADC<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x6e] = &OpCodesTable::OpROR<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x6f] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x70] = &OpCodesTable::OpBVS<&OpCodesTable::AddressingModeRelative>;
    opcodes_[0x71] = &OpCodesTable::OpADC<&OpCodesTable::AddressingModeIndirectY>;
    opcodes_[0x72] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x73] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x74] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x75] = &OpCodesTable::OpADC<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x76] = &OpCodesTable::OpROR<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x77] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x78] = &OpCodesTable::OpSEI<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x79] = &OpCodesTable::OpADC<&OpCodesTable::AddressingModeAbsoluteY>;
    opcodes_[0x7a] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x7b] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x7c] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0x7d] = &OpCodesTable::OpADC<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0x7e] = &OpCodesTable::OpROR<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0x7f] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x80] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0x81] = &OpCodesTable::OpSTA<&OpCodesTable::AddressingModeIndirectX>;
    opcodes_[0x82] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0x83] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x84] = &OpCodesTable::OpSTY<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x85] = &OpCodesTable::OpSTA<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x86] = &OpCodesTable::OpSTX<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0x87] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x88] = &OpCodesTable::OpDEY<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x89] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0x8a] = &OpCodesTable::OpTXA<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x8b] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x8c] = &OpCodesTable::OpSTY<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x8d] = &OpCodesTable::OpSTA<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x8e] = &OpCodesTable::OpSTX<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0x8f] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x90] = &OpCodesTable::OpBCC<&OpCodesTable::AddressingModeRelative>;
    opcodes_[0x91] = &OpCodesTable::OpSTA<&OpCodesTable::AddressingModeIndirectY>;
    opcodes_[0x92] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x93] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x94] = &OpCodesTable::OpSTY<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x95] = &OpCodesTable::OpSTA<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0x96] = &OpCodesTable::OpSTX<&OpCodesTable::AddressingModeZeroPageY>;
    opcodes_[0x97] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x98] = &OpCodesTable::OpTYA<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x99] = &OpCodesTable::OpSTA<&OpCodesTable::AddressingModeAbsoluteY>;
    opcodes_[0x9a] = &OpCodesTable::OpTXS<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0x9b] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x9c] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x9d] = &OpCodesTable::OpSTA<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0x9e] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0x9f] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xa0] = &OpCodesTable::OpLDY<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0xa1] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeIndirectX>;
    opcodes_[0xa2] = &OpCodesTable::OpLDX<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0xa3] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xa4] = &OpCodesTable::OpLDY<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0xa5] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0xa6] = &OpCodesTable::OpLDX<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0xa7] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xa8] = &OpCodesTable::OpTAY<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0xa9] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0xaa] = &OpCodesTable::OpTAX<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0xab] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xac] = &OpCodesTable::OpLDY<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0xad] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0xae] = &OpCodesTable::OpLDX<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0xaf] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xb0] = &OpCodesTable::OpBCS<&OpCodesTable::AddressingModeRelative>;
    opcodes_[0xb1] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeIndirectY>;
    opcodes_[0xb2] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xb3] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xb4] = &OpCodesTable::OpLDY<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0xb5] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0xb6] = &OpCodesTable::OpLDX<&OpCodesTable::AddressingModeZeroPageY>;
    opcodes_[0xb7] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xb8] = &OpCodesTable::OpCLV<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0xb9] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeAbsoluteY>;
    opcodes_[0xba] = &OpCodesTable::OpTSX<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0xbb] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xbc] = &OpCodesTable::OpLDY<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0xbd] = &OpCodesTable::OpLDA<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0xbe] = &OpCodesTable::OpLDX<&OpCodesTable::AddressingModeAbsoluteY>;
    opcodes_[0xbf] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xc0] = &OpCodesTable::OpCPY<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0xc1] = &OpCodesTable::OpCMP<&OpCodesTable::AddressingModeIndirectX>;
    opcodes_[0xc2] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0xc3] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xc4] = &OpCodesTable::OpCPY<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0xc5] = &OpCodesTable::OpCMP<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0xc6] = &OpCodesTable::OpDEC<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0xc7] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xc8] = &OpCodesTable::OpINY<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0xc9] = &OpCodesTable::OpCMP<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0xca] = &OpCodesTable::OpDEX<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0xcb] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xcc] = &OpCodesTable::OpCPY<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0xcd] = &OpCodesTable::OpCMP<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0xce] = &OpCodesTable::OpDEC<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0xcf] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xd0] = &OpCodesTable::OpBNE<&OpCodesTable::AddressingModeRelative>;
    opcodes_[0xd1] = &OpCodesTable::OpCMP<&OpCodesTable::AddressingModeIndirectY>;
    opcodes_[0xd2] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xd3] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xd4] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0xd5] = &OpCodesTable::OpCMP<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0xd6] = &OpCodesTable::OpDEC<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0xd7] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xd8] = &OpCodesTable::OpCLD<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0xd9] = &OpCodesTable::OpCMP<&OpCodesTable::AddressingModeAbsoluteY>;
    opcodes_[0xda] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0xdb] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xdc] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0xdd] = &OpCodesTable::OpCMP<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0xde] = &OpCodesTable::OpDEC<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0xdf] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xe0] = &OpCodesTable::OpCPX<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0xe1] = &OpCodesTable::OpSBC<&OpCodesTable::AddressingModeIndirectX>;
    opcodes_[0xe2] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0xe3] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xe4] = &OpCodesTable::OpCPX<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0xe5] = &OpCodesTable::OpSBC<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0xe6] = &OpCodesTable::OpINC<&OpCodesTable::AddressingModeZeroPage>;
    opcodes_[0xe7] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xe8] = &OpCodesTable::OpINX<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0xe9] = &OpCodesTable::OpSBC<&OpCodesTable::AddressingModeImmediate>;
    opcodes_[0xea] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0xeb] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xec] = &OpCodesTable::OpCPX<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0xed] = &OpCodesTable::OpSBC<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0xee] = &OpCodesTable::OpINC<&OpCodesTable::AddressingModeAbsolute>;
    opcodes_[0xef] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xf0] = &OpCodesTable::OpBEQ<&OpCodesTable::AddressingModeRelative>;
    opcodes_[0xf1] = &OpCodesTable::OpSBC<&OpCodesTable::AddressingModeIndirectY>;
    opcodes_[0xf2] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xf3] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xf4] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0xf5] = &OpCodesTable::OpSBC<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0xf6] = &OpCodesTable::OpINC<&OpCodesTable::AddressingModeZeroPageX>;
    opcodes_[0xf7] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xf8] = &OpCodesTable::OpSED<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0xf9] = &OpCodesTable::OpSBC<&OpCodesTable::AddressingModeAbsoluteY>;
    opcodes_[0xfa] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeImplied>;
    opcodes_[0xfb] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
    opcodes_[0xfc] = &OpCodesTable::OpNOP<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0xfd] = &OpCodesTable::OpSBC<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0xfe] = &OpCodesTable::OpINC<&OpCodesTable::AddressingModeAbsoluteX>;
    opcodes_[0xff] = &OpCodesTable::OpNotImplemented<&OpCodesTable::AddressingModeNone>;
}

inline void OpCodesTable::UpdateNegativeFlag(CPU *cpu, uint8_t result)
{
    if (result >= 0b10000000)
        cpu->SetStatusRegisterFlag(kNegativeFlag);
    else
        cpu->ClearStatusRegisterFlag(kNegativeFlag);
}

inline void OpCodesTable::UpdateZeroFlag(CPU *cpu, uint8_t result)
{
    if (result == 0x00)
        cpu->SetStatusRegisterFlag(kZeroFlag);
    else
        cpu->ClearStatusRegisterFlag(kZeroFlag);
}

inline void OpCodesTable::UpdateCarryFlag(CPU *cpu, uint16_t result)
{
    if ((result & 0xFF00) > 0x0000)
        cpu->SetStatusRegisterFlag(kCarryFlag);
    else
        cpu->ClearStatusRegisterFlag(kCarryFlag);
}

inline void OpCodesTable::UpdateOverflowFlag(CPU *cpu, Byte a, Byte m, Byte r)
{
    /*
    Based off discussions below:
    http://www.6502.org/tutorials/vflag.html
    https://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
    */
    if ((a ^ r) & (m ^ r) & 0x80)
        cpu->SetStatusRegisterFlag(kOverflowFlag);
    else
        cpu->ClearStatusRegisterFlag(kOverflowFlag);
}

uint8_t OpCodesTable::RunOpCode(CPU *cpu, Byte opcode)
{
    ((*this).*(opcodes_[opcode]))(cpu, opcode);
    return 0;
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeImplied(CPU *cpu)
{
    return {0, false, 2};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeAccumulator(CPU *cpu)
{
    return {cpu->GetAccumulator(), false, 2};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeImmediate(CPU *cpu)
{
    auto immediate_val = cpu->GetCurrentOpCode();
    cpu->AdvanceProgramCounter();
    return {immediate_val, false, 2};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeAbsolute(CPU *cpu)
{
    auto absolute_address = cpu->GetMemoryWord(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    cpu->AdvanceProgramCounter();
    return {absolute_address, true, 4};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeAbsoluteX(CPU *cpu)
{
    uint8_t cycles = 4;
    auto absolute_address = cpu->GetMemoryWord(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    cpu->AdvanceProgramCounter();
    uint32_t indexed_address = (absolute_address + cpu->GetXIndex());

    // Crossed page boundry increasing cycle count by 1.
    if ((indexed_address >> 8) > (absolute_address >> 8))
        cycles++;

    return {(uint16_t)(indexed_address & 0x0000FFFF), true, cycles};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeAbsoluteY(CPU *cpu)
{
    uint8_t cycles = 4;
    auto absolute_address = cpu->GetMemoryWord(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    cpu->AdvanceProgramCounter();
    uint32_t indexed_address = (absolute_address + cpu->GetYIndex());

    // Crossed page boundry increasing cycle count by 1.
    if ((indexed_address >> 8) > (absolute_address >> 8))
        cycles++;

    return {(uint16_t)(indexed_address & 0x0000FFFF), true, cycles};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeZeroPage(CPU *cpu)
{
    auto zero_page_addr = cpu->GetMemoryByte(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    return {zero_page_addr, true, 3};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeZeroPageX(CPU *cpu)
{
    Word zero_page_addr = cpu->GetMemoryByte(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    zero_page_addr = (zero_page_addr + cpu->GetXIndex()) & 0x00FF;
    return {zero_page_addr, true, 4};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeZeroPageY(CPU *cpu)
{
    Word zero_page_addr = cpu->GetMemoryByte(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    zero_page_addr = (zero_page_addr + cpu->GetYIndex()) & 0x00FF;
    return {zero_page_addr, true, 4};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeIndirectX(CPU *cpu)
{
    Byte indirect_addr = cpu->GetMemoryByte(cpu->GetProgramCounter());
    Byte lo_byte = (indirect_addr + cpu->GetXIndex()) & 0x00FF;
    Byte high_byte = (indirect_addr + cpu->GetXIndex() + 1) & 0x00FF;
    Byte lo_address = cpu->GetMemoryByte(lo_byte);
    Byte high_address = cpu->GetMemoryByte(high_byte);
    Word addr = (high_address << 8) | lo_address;

    cpu->AdvanceProgramCounter();
    return {addr, true, 6};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeIndirectY(CPU *cpu)
{
    uint8_t cycles = 5;
    Byte indirect_addr = cpu->GetMemoryByte(cpu->GetProgramCounter());
    Byte lo_byte = (indirect_addr)&0x00FF;
    Byte high_byte = (indirect_addr + 1) & 0x00FF;
    Byte lo_address = cpu->GetMemoryByte(lo_byte);
    Byte high_address = cpu->GetMemoryByte(high_byte);
    Word dereferenced_addr = ((high_address << 8) | lo_address);
    Word addr = ((high_address << 8) | lo_address) + cpu->GetYIndex();

    cpu->AdvanceProgramCounter();

    if ((addr >> 8) > (dereferenced_addr >> 8))
        cycles++;

    return {addr, true, cycles};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeAbsoluteIndirect(CPU *cpu)
{
    Word indirect_addr = cpu->GetMemoryWord(cpu->GetProgramCounter());
    Word addr = cpu->GetMemoryWord(indirect_addr);
    if ((indirect_addr & 0x00FF) == 0xFF)
    {
        Byte lsb = cpu->GetMemoryByte(indirect_addr);
        Byte msb = cpu->GetMemoryByte(indirect_addr & 0xFF00);
        addr = (msb << 8) | lsb;
    }

    cpu->AdvanceProgramCounter();
    cpu->AdvanceProgramCounter();
    return {addr, true, 5};
}

OpCodesTable::AddressingVal OpCodesTable::AddressingModeRelative(CPU *cpu)
{
    uint8_t cycles = 2;
    Byte offset = cpu->GetMemoryByte(cpu->GetProgramCounter());
    cpu->AdvanceProgramCounter();
    uint16_t program_counter = cpu->GetProgramCounter();
    uint16_t relative_address = program_counter + ((int8_t)offset);
    if (((program_counter ^ relative_address) & 0xFF00) != 0x0000)
        cycles++;

    return {relative_address, false, cycles};
}

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpNotImplemented(CPU *cpu, Byte opcode)
{
    throw std::runtime_error("OpCode Not Implemented");
};

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpADC(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    if (address_mode_val.is_address)
        address_mode_val.value = cpu->GetMemoryByte(address_mode_val.value);

    Byte accumulator = cpu->GetAccumulator();
    Byte memory = address_mode_val.value;
    Word result = accumulator + memory + cpu->GetStatusRegister().flags.c;
    cpu->SetAccumulator((Byte)(result & 0x00FF));
    UpdateNegativeFlag(cpu, result);
    UpdateZeroFlag(cpu, result);
    UpdateCarryFlag(cpu, result);
    UpdateOverflowFlag(cpu, accumulator, memory, result);
};

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpAND(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    if (address_mode_val.is_address)
        address_mode_val.value = cpu->GetMemoryByte(address_mode_val.value);

    Word result = cpu->GetAccumulator() & address_mode_val.value;
    cpu->SetAccumulator((Byte)(result & 0x00FF));
    UpdateNegativeFlag(cpu, result);
    UpdateZeroFlag(cpu, result);
};

// BRK
// cycle 1: fetch opcode, increment pc_
// cycle 2: read next instruction byte(unused), increment pc_
// cycle 3: push PCH on stack with B flag set, decrement sp_
// cycle 4: puch PCL on stack, decrement sp_
// cycle 5: push status register on stack, decrement sp_
// cycle 6: fetch PCL at $FFFE
// cycle 7: fetch PCH at $FFFF
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpBRK(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->SetStatusRegisterFlag(kBreakFlag);

    Byte pc_h = (cpu->GetProgramCounter() & 0xFF00) >> 8;
    Byte pc_l = cpu->GetProgramCounter() & 0xFF;

    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), pc_h);
    cpu->DecrementStackPointer();

    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), pc_l);
    cpu->DecrementStackPointer();

    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), cpu->GetStatusRegister().data);
    cpu->DecrementStackPointer();

    Word new_pc = cpu->GetMemoryWord(0xFFFE);
    cpu->SetProgramCounter(new_pc);

    cpu->IncreaseCycleCount(address_mode_val.cycles + 5); // 7 cycles total
}

// PHP
// Push status register on stack, decrement stack pointer
// set status bits 4 and 5
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpPHP(CPU *cpu, Byte opcode)
{
    StatusRegister copied_sr = cpu->GetStatusRegister();
    copied_sr.flags.b = 1;
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), copied_sr.data);
    cpu->DecrementStackPointer();
    cpu->IncreaseCycleCount(address_mode_val.cycles + 1); // 3 cycles total
}

// PHA
// Push accumulator on stack, decrement stack pointer
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpPHA(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), cpu->GetAccumulator());
    cpu->DecrementStackPointer();
    cpu->IncreaseCycleCount(address_mode_val.cycles + 1); // 3 cycles total
}

// PLA
// increment stack pointer, pull top of stack and store in accumulator
// zero flag set if copied value is 0, otherwise cleared
// negative flag is set to 7th bit of copied value
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpPLA(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncrementStackPointer();
    Byte copied_value = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());
    cpu->SetAccumulator(copied_value);

    // set zero flag
    UpdateZeroFlag(cpu, copied_value);

    // negative flag
    UpdateNegativeFlag(cpu, copied_value);

    cpu->IncreaseCycleCount(address_mode_val.cycles + 2); // 4 cycles total
}

// PLP
// increment stack pointer, pull top of stack and store in status register
// set bit 5 to 1
// set bit 4 to 0
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpPLP(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncrementStackPointer();
    Byte copied_value = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());

    copied_value |= 0b00100000;
    StatusRegister new_sr{copied_value};
    new_sr.flags.b = 0;

    cpu->SetStatusRegister(new_sr.data);
    cpu->IncreaseCycleCount(address_mode_val.cycles + 2); // 4 cycles total
}

// RTI
// pop top of stack and store in status register
// pop top of stack and store in pc_l
// pop top of stack and store in pc_h
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpRTI(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncrementStackPointer();
    Byte copied_value = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());
    copied_value |= 0b00100000;
    StatusRegister new_sr{copied_value};
    new_sr.flags.b = 0;
    cpu->SetStatusRegister(new_sr.data);

    cpu->IncrementStackPointer();
    Byte pc_l = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());

    cpu->IncrementStackPointer();
    Byte pc_h = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());

    cpu->SetProgramCounter((pc_h << 8) | pc_l);

    cpu->IncreaseCycleCount(address_mode_val.cycles + 4); // 6 cycles total
}

// RTS
// pop top of stack and store in pc_l
// pop top of stack and store in pc_h
// increment program counter
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpRTS(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncrementStackPointer();
    Byte pc_l = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());

    cpu->IncrementStackPointer();
    Byte pc_h = cpu->GetMemoryByte(0x100 + cpu->GetStackPointer());

    cpu->SetProgramCounter((pc_h << 8) | pc_l);
    cpu->AdvanceProgramCounter();

    cpu->IncreaseCycleCount(address_mode_val.cycles + 4); // 6 cycles total
}

// JSR
// push pc_h on stack, decrement stack pointer
// push pc_l on stack, decrement stack pointer
// copy low address byte to pc_l
// copy high address byte to pc_h
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpJSR(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);

    // decrement pc to preserve last address before jump. pc will increment automatically with RTS
    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), (Byte)((cpu->GetProgramCounter() - 1) >> 8));
    cpu->DecrementStackPointer();

    cpu->WriteMemory(0x100 + cpu->GetStackPointer(), (Byte)((cpu->GetProgramCounter() - 1) & 0xFF));
    cpu->DecrementStackPointer();

    cpu->SetProgramCounter(address_mode_val.value);
    cpu->IncreaseCycleCount(address_mode_val.cycles + 2); // 6 cycles total
}

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpCMP(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    if (address_mode_val.is_address)
        address_mode_val.value = cpu->GetMemoryByte(address_mode_val.value);

    Byte accumulator = cpu->GetAccumulator();
    Byte memory = (address_mode_val.value ^ 0xFF);
    Word result = accumulator + memory + 1;
    UpdateNegativeFlag(cpu, result);
    UpdateZeroFlag(cpu, result);
    UpdateCarryFlag(cpu, result);
};

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpEOR(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    if (address_mode_val.is_address)
        address_mode_val.value = cpu->GetMemoryByte(address_mode_val.value);

    Word result = cpu->GetAccumulator() ^ address_mode_val.value;
    cpu->SetAccumulator((Byte)(result & 0x00FF));
    UpdateNegativeFlag(cpu, result);
    UpdateZeroFlag(cpu, result);
};

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpLDA(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    if (address_mode_val.is_address)
        address_mode_val.value = cpu->GetMemoryWord(address_mode_val.value);

    cpu->SetAccumulator(address_mode_val.value);
    UpdateNegativeFlag(cpu, address_mode_val.value);
    UpdateZeroFlag(cpu, address_mode_val.value);
};

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpNOP(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);
};

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpORA(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    if (address_mode_val.is_address)
        address_mode_val.value = cpu->GetMemoryByte(address_mode_val.value);

    Word result = cpu->GetAccumulator() | address_mode_val.value;
    cpu->SetAccumulator((Byte)(result & 0x00FF));
    UpdateNegativeFlag(cpu, result);
    UpdateZeroFlag(cpu, result);
};

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpSBC(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    if (address_mode_val.is_address)
        address_mode_val.value = cpu->GetMemoryByte(address_mode_val.value);

    /*
    Based off discussions below for purposes of handling the various overflow and carry flags:
    http://www.6502.org/tutorials/vflag.html
    https://www.righto.com/2012/12/the-6502-overflow-flag-explained.html
    result = accumulator - memory - ~borrow
    result = accumulator + ~memory + borrow
    */
    Byte accumulator = cpu->GetAccumulator();
    Byte memory = (address_mode_val.value ^ 0xFF);
    Word result = accumulator + memory + cpu->GetStatusRegister().flags.c;
    cpu->SetAccumulator((Byte)(result & 0x00FF));
    UpdateNegativeFlag(cpu, result);
    UpdateZeroFlag(cpu, result);
    UpdateCarryFlag(cpu, result);
    UpdateOverflowFlag(cpu, accumulator, memory, result);
};

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpSTA(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);

    // Handling manually as cycle counts differ general pattern.
    switch ((opcode & 0b00011100) >> 2)
    {
    case 0b000: // Indirect X
        cpu->IncreaseCycleCount(6);
        break;

    case 0b001: // ZeroPage
        cpu->IncreaseCycleCount(3);
        break;

    case 0b011: // Absolute
        cpu->IncreaseCycleCount(4);
        break;

    case 0b100: // Indirect Y
        cpu->IncreaseCycleCount(6);
        break;

    case 0b101: // ZeroPage X
        cpu->IncreaseCycleCount(4);
        break;

    case 0b110: // Absolute Y
        cpu->IncreaseCycleCount(5);
        break;

    case 0b111: // Absolute X
        cpu->IncreaseCycleCount(5);
        break;
    }

    cpu->WriteMemory(address_mode_val.value, cpu->GetAccumulator());
};

// Load register X from memory
// zero flag set if loaded value is 0, otherwise cleared
// negative flag is set to 7th bit of loaded value
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpLDX(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    if (address_mode_val.is_address)
        address_mode_val.value = cpu->GetMemoryWord(address_mode_val.value);

    const auto loaded_value = address_mode_val.value;
    UpdateZeroFlag(cpu, loaded_value);
    UpdateNegativeFlag(cpu, loaded_value);

    cpu->SetXIndex(loaded_value);
};

template <OpCodesTable::AddressMode A>
void OpCodesTable::OpSTX(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);
    cpu->WriteMemory(address_mode_val.value, cpu->GetXIndex());
};

// SED
// Set decimal mode in status register
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpSED(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->SetStatusRegisterFlag(kDecimalFlag);
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// CLD
// Clears decimal mode in status register
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpCLD(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->ClearStatusRegisterFlag(kDecimalFlag);
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// SEI
// Set interrupt disable flag in status register
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpSEI(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->SetStatusRegisterFlag(kInterruptFlag);
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// CLI
// Clear interrupt disable flag in status register
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpCLI(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->ClearStatusRegisterFlag(kInterruptFlag);
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// SEC
// Set carry flag in status register
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpSEC(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->SetStatusRegisterFlag(kCarryFlag);
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// CLC
// Clear carry flag in status register
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpCLC(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->ClearStatusRegisterFlag(kCarryFlag);
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// CLV
// Clear overflow flag in status register
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpCLV(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->ClearStatusRegisterFlag(kOverflowFlag);
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// ASL
// Shifts accumulator or the address memory location 1 bit to the left
// Bit 0 is set to 0 and bit 7 moves to carry flag
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpASL(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    uint16_t address = 0;
    if (address_mode_val.is_address)
    {
        address = address_mode_val.value;
        address_mode_val.value = cpu->GetMemoryByte(address_mode_val.value);
    }

    const uint16_t leftShiftedValue = address_mode_val.value << 1;

    UpdateCarryFlag(cpu, leftShiftedValue);
    UpdateZeroFlag(cpu, leftShiftedValue);
    UpdateNegativeFlag(cpu, leftShiftedValue);
    if (address_mode_val.is_address)
    {
        cpu->WriteMemory(address, (Byte)leftShiftedValue);
        cpu->IncreaseCycleCount(2);
    }
    else
    {
        cpu->SetAccumulator(leftShiftedValue);
    }
    if (((opcode & 0b00011100) >> 2) == 0b111) // Absolute X needs an extra clock cycle
    {
        cpu->IncreaseCycleCount(1);
    }
}

// LSR
// Shifts accumulator or the address memory location 1 bit to the right
// Bit 7 is set to 0 and bit 0 moves to carry flag
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpLSR(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    uint16_t address = 0;
    if (address_mode_val.is_address)
    {
        address = address_mode_val.value;
        address_mode_val.value = cpu->GetMemoryByte(address_mode_val.value);
    }
    if (address_mode_val.value & 1)
    {
        cpu->SetStatusRegisterFlag(kCarryFlag);
    }
    else
    {
        cpu->ClearStatusRegisterFlag(kCarryFlag);
    }
    const uint16_t rightShiftedValue = address_mode_val.value >> 1;

    UpdateZeroFlag(cpu, rightShiftedValue);
    cpu->ClearStatusRegisterFlag(kNegativeFlag);

    if (address_mode_val.is_address)
    {
        cpu->WriteMemory(address, (Byte)rightShiftedValue);
        cpu->IncreaseCycleCount(2);
    }
    else
    {
        cpu->SetAccumulator(rightShiftedValue);
    }
    if (((opcode & 0b00011100) >> 2) == 0b111) // Absolute X needs an extra clock cycle
    {
        cpu->IncreaseCycleCount(1);
    }
}

// ROL
// Shifts accumulator or the address memory location 1 bit to the left
// Bit 0 takes the value of the carry flag and bit 7 moves to the carry flag
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpROL(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    uint16_t address = 0;
    if (address_mode_val.is_address)
    {
        address = address_mode_val.value;
        address_mode_val.value = cpu->GetMemoryByte(address_mode_val.value);
    }

    const uint16_t leftShiftedValue = (address_mode_val.value << 1) | cpu->GetStatusRegister().flags.c;

    UpdateCarryFlag(cpu, leftShiftedValue);
    UpdateZeroFlag(cpu, leftShiftedValue);
    UpdateNegativeFlag(cpu, leftShiftedValue);
    if (address_mode_val.is_address)
    {
        cpu->WriteMemory(address, (Byte)leftShiftedValue);
        cpu->IncreaseCycleCount(2);
    }
    else
    {
        cpu->SetAccumulator(leftShiftedValue);
    }
    if (((opcode & 0b00011100) >> 2) == 0b111) // Absolute X needs an extra clock cycle
    {
        cpu->IncreaseCycleCount(1);
    }
}

// ROR
// Shifts accumulator or the address memory location 1 bit to the right
// Bit 7 takes the value of the carry flag and bit 0 moves to the carry flag
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpROR(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    uint16_t address = 0;
    if (address_mode_val.is_address)
    {
        address = address_mode_val.value;
        address_mode_val.value = cpu->GetMemoryByte(address_mode_val.value);
    }
    const uint16_t rightShiftedValue = (address_mode_val.value >> 1) | (cpu->GetStatusRegister().flags.c << 7);

    if (address_mode_val.value & 1)
    {
        cpu->SetStatusRegisterFlag(kCarryFlag);
    }
    else
    {
        cpu->ClearStatusRegisterFlag(kCarryFlag);
    }

    UpdateZeroFlag(cpu, rightShiftedValue);
    UpdateNegativeFlag(cpu, rightShiftedValue);

    if (address_mode_val.is_address)
    {
        cpu->WriteMemory(address, (Byte)rightShiftedValue);
        cpu->IncreaseCycleCount(2);
    }
    else
    {
        cpu->SetAccumulator(rightShiftedValue);
    }
    if (((opcode & 0b00011100) >> 2) == 0b111) // Absolute X needs an extra clock cycle
    {
        cpu->IncreaseCycleCount(1);
    }
}

// INY
// increment value in Y register, wrap around 0xff to 0x00
// zero flag set if new value is 0, otherwise cleared
// negative flag updated to value of 7th bit
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpINY(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);

    if (cpu->GetYIndex() == 0xFF)
    {
        cpu->SetYIndex(0x00);
        cpu->SetStatusRegisterFlag(kZeroFlag);
        cpu->ClearStatusRegisterFlag(kNegativeFlag);
    }
    else
    {
        Byte result = cpu->GetYIndex() + 1;
        cpu->SetYIndex(result);
        cpu->ClearStatusRegisterFlag(kZeroFlag);
        UpdateNegativeFlag(cpu, result);
    }

    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// INX
// increment value in X register, wrap around 0xff to 0x00
// zero flag set if new value is 0, otherwise cleared
// negative flag updated to value of 7th bit
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpINX(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);

    if (cpu->GetXIndex() == 0xFF)
    {
        cpu->SetXIndex(0x00);
        cpu->SetStatusRegisterFlag(kZeroFlag);
        cpu->ClearStatusRegisterFlag(kNegativeFlag);
    }
    else
    {
        Byte result = cpu->GetXIndex() + 1;
        cpu->SetXIndex(result);
        cpu->ClearStatusRegisterFlag(kZeroFlag);
        UpdateNegativeFlag(cpu, result);
    }

    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// DEY
// decrement value in Y register, wrap around 0x00 to 0xff
// zero flag set if new value is 0, otherwise cleared
// negative flag updated to value of 7th bit
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpDEY(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    Byte result;
    if (cpu->GetYIndex() == 0x00)
    {
        result = 0xff;
    }
    else
    {
        result = cpu->GetYIndex() - 1;
    }
    cpu->SetYIndex(result);
    UpdateNegativeFlag(cpu, result);
    UpdateZeroFlag(cpu, result);
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// TAY
// copy value in Accumulator to Y register
// zero flag set if copied value is 0, otherwise cleared
// negative flag updated to value of 7th bit of copied value
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpTAY(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);

    cpu->SetYIndex(cpu->GetAccumulator());
    UpdateNegativeFlag(cpu, cpu->GetYIndex());
    UpdateZeroFlag(cpu, cpu->GetYIndex());

    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// TYA
// copy value in Y register to Accumulator
// zero flag set if copied value is 0, otherwise cleared
// negative flag updated to value of 7th bit of copied value
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpTYA(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);

    cpu->SetAccumulator(cpu->GetYIndex());
    UpdateNegativeFlag(cpu, cpu->GetAccumulator());
    UpdateZeroFlag(cpu, cpu->GetAccumulator());

    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// CPX
// subtract byte specified at operand from value in X register
// the result is not stored anywhere
// carry flag is set if value in X register >= operand byte, otherwise cleared
// zero flag set if value in X register == operand byte, otherwise cleared
// negative flag set to 7th bit of result
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpCPX(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    if (address_mode_val.is_address)
    {
        address_mode_val.value = cpu->GetMemoryByte(address_mode_val.value);
    }

    if (cpu->GetXIndex() >= address_mode_val.value)
    {
        cpu->SetStatusRegisterFlag(kCarryFlag);
    }
    else
    {
        cpu->ClearStatusRegisterFlag(kCarryFlag);
    }

    Byte result = cpu->GetXIndex() - address_mode_val.value;
    UpdateNegativeFlag(cpu, result);
    UpdateZeroFlag(cpu, result);

    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// CPY
// subtract byte specified at operand from value in Y register
// the result is not stored anywhere
// carry flag is set if value in Y register >= operand byte, otherwise cleared
// zero flag set if value in Y register == operand byte, otherwise cleared
// negative flag set to 7th bit of result
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpCPY(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    if (address_mode_val.is_address)
    {
        address_mode_val.value = cpu->GetMemoryByte(address_mode_val.value);
    }

    if (cpu->GetYIndex() >= address_mode_val.value)
    {
        cpu->SetStatusRegisterFlag(kCarryFlag);
    }
    else
    {
        cpu->ClearStatusRegisterFlag(kCarryFlag);
    }

    Byte result = cpu->GetYIndex() - address_mode_val.value;
    UpdateNegativeFlag(cpu, result);
    UpdateZeroFlag(cpu, result);

    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// BEQ
// branch if the zero flag is set
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpBEQ(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    if (cpu->GetStatusRegister().flags.z == 1)
    {
        cpu->SetProgramCounter(address_mode_val.value);
        cpu->IncreaseCycleCount(1);
    }
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// BNE
// branch if the zero flag cleared
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpBNE(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    if (cpu->GetStatusRegister().flags.z == 0)
    {
        cpu->SetProgramCounter(address_mode_val.value);
        cpu->IncreaseCycleCount(1);
    }
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// BCS
// branch if the carry flag is set
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpBCS(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    if (cpu->GetStatusRegister().flags.c == 1)
    {
        cpu->SetProgramCounter(address_mode_val.value);
        cpu->IncreaseCycleCount(1);
    }
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// BCC
// branch if the carry flag is cleared
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpBCC(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    if (cpu->GetStatusRegister().flags.c == 0)
    {
        cpu->SetProgramCounter(address_mode_val.value);
        cpu->IncreaseCycleCount(1);
    }
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// BVS
// branch if the overflow flag is set
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpBVS(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    if (cpu->GetStatusRegister().flags.o == 1)
    {
        cpu->SetProgramCounter(address_mode_val.value);
        cpu->IncreaseCycleCount(1);
    }
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// BVC
// branch if the overflow flag is cleared
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpBVC(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    if (cpu->GetStatusRegister().flags.o == 0)
    {
        cpu->SetProgramCounter(address_mode_val.value);
        cpu->IncreaseCycleCount(1);
    }
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// BMI
// branch if the negative flag is set
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpBMI(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    if (cpu->GetStatusRegister().flags.n == 1)
    {
        cpu->SetProgramCounter(address_mode_val.value);
        cpu->IncreaseCycleCount(1);
    }

    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// BPL
// branch if the negative flag is NOT set
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpBPL(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    if (cpu->GetStatusRegister().flags.n == 0)
    {
        cpu->SetProgramCounter(address_mode_val.value);
        cpu->IncreaseCycleCount(1);
    }

    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// INC
// Increment the value in memory by 1
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpINC(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    const Byte incrementedValue = cpu->GetMemoryByte(address_mode_val.value) + 1;

    if (((opcode & 0b00011100) >> 2) == 0b111) // Absolute X needs an extra clock cycle
    {
        cpu->IncreaseCycleCount(1);
    }

    UpdateZeroFlag(cpu, incrementedValue);
    UpdateNegativeFlag(cpu, incrementedValue);
    cpu->WriteMemory(address_mode_val.value, (Byte)incrementedValue);
    cpu->IncreaseCycleCount(2); // 2 cycles to write back to memory
}

// DEC
// Decrement the value in memory by 1
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpDEC(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    const Byte decrementedValue = cpu->GetMemoryByte(address_mode_val.value) - 1;

    if (((opcode & 0b00011100) >> 2) == 0b111) // Absolute X needs an extra clock cycle
    {
        cpu->IncreaseCycleCount(1);
    }

    UpdateZeroFlag(cpu, decrementedValue);
    UpdateNegativeFlag(cpu, decrementedValue);
    cpu->WriteMemory(address_mode_val.value, (Byte)decrementedValue);
    cpu->IncreaseCycleCount(2); // 2 cycles to write back to memory
}

// DEX
// Decrement the value in index register X by 1
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpDEX(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    const Byte decrementedValue = cpu->GetXIndex() - 1;

    UpdateZeroFlag(cpu, decrementedValue);
    UpdateNegativeFlag(cpu, decrementedValue);
    cpu->SetXIndex(decrementedValue);
}

// TXA
// Transfer index X to accumulator
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpTXA(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    const Byte value = cpu->GetXIndex();
    cpu->SetAccumulator(value);

    UpdateZeroFlag(cpu, value);
    UpdateNegativeFlag(cpu, value);
}

// TAX
// Transfer accumulator to index X
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpTAX(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    const Byte value = cpu->GetAccumulator();
    cpu->SetXIndex(value);

    UpdateZeroFlag(cpu, value);
    UpdateNegativeFlag(cpu, value);
}

// TXS
// Transfer index X to stack pointer
// Does not change status flags
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpTXS(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);
    cpu->SetStackPointer(cpu->GetXIndex());
}

// TSX
// Transfer stack point to index X
// Updates N and Z flags depending on value transferred into X
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpTSX(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->IncreaseCycleCount(address_mode_val.cycles);

    const Byte value = cpu->GetStackPointer();
    cpu->SetXIndex(value);

    UpdateZeroFlag(cpu, value);
    UpdateNegativeFlag(cpu, value);
}

// JMP
// sets the program counter to the memory location specified by the operand
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpJMP(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->SetProgramCounter(address_mode_val.value);

    if (address_mode_val.cycles == 4) // Absolute
    {
        // JMP uses 3 cycles for Absolute
        cpu->IncreaseCycleCount(3);
    }
    else // Absolute Indirect
    {
        cpu->IncreaseCycleCount(address_mode_val.cycles);
    }
};

// BIT
// perform bitwise AND between accumulator and byte in operand
// zero flag set if result of AND is zero, otherwise cleared
// overflow flag updated to equal bit #6 of operand byte
// negative flag updated to equal bit #7 of operand byte
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpBIT(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);
    address_mode_val.value = cpu->GetMemoryByte(address_mode_val.value);
    auto result = cpu->GetAccumulator() & address_mode_val.value;

    UpdateNegativeFlag(cpu, address_mode_val.value);
    UpdateZeroFlag(cpu, result);

    // set overflow flag
    if ((address_mode_val.value & (1 << 6)) >> 6)
    {
        cpu->SetStatusRegisterFlag(kOverflowFlag);
    }
    else
    {
        cpu->ClearStatusRegisterFlag(kOverflowFlag);
    }

    cpu->IncreaseCycleCount(address_mode_val.cycles);
};

// STY
// Copy value in Y register to specified memory location
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpSTY(CPU *cpu, Byte opcode)
{
    struct AddressingVal address_mode_val = ((*this).*A)(cpu);
    cpu->WriteMemory(address_mode_val.value, cpu->GetYIndex());
    cpu->IncreaseCycleCount(address_mode_val.cycles);
}

// Load register Y from memory
// zero flag set if loaded value is 0, otherwise cleared
// negative flag is set to 7th bit of loaded value
template <OpCodesTable::AddressMode A>
void OpCodesTable::OpLDY(CPU *cpu, Byte opcode)
{
    struct OpCodesTable::AddressingVal address_mode_val = ((*this).*A)(cpu);
    if (address_mode_val.is_address)
        address_mode_val.value = cpu->GetMemoryWord(address_mode_val.value);

    const auto loaded_value = address_mode_val.value;

    UpdateZeroFlag(cpu, loaded_value);
    UpdateNegativeFlag(cpu, loaded_value);

    cpu->SetYIndex(loaded_value);
    cpu->IncreaseCycleCount(address_mode_val.cycles);
};
