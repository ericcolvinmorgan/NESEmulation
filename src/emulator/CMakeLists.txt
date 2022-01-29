add_library(emulator emulator.cpp cpu.cpp raw_memory_accessor.cpp opcodes_table.cpp)
target_include_directories (emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})