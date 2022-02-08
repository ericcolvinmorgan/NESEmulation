add_library(6502_emulator 6502/emulator.cpp cpu.cpp memory_accessor_interface.cpp raw_memory_accessor.cpp opcodes_table.cpp)
target_include_directories (6502_emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(nes_emulator emulator.cpp cpu.cpp memory_accessor_interface.cpp nes_ppu_memory_accessor.cpp nes_cpu_memory_accessor.cpp raw_memory_accessor.cpp opcodes_table.cpp ppu.cpp)
target_include_directories (nes_emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})