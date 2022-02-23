cmake_minimum_required(VERSION 3.5)
project(nesemulation VERSION 0.1.0)

add_subdirectory(src/emulator)
add_subdirectory(src/interface)
add_subdirectory(test)

target_include_directories(6502_emulator PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )


target_include_directories(nes_emulator PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
