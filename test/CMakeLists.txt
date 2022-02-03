if(NOT DEFINED EMSCRIPTEN)
    # find_package(Catch2 REQUIRED)
    Include(FetchContent)

    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        v3.0.0-preview4
    )
    
    FetchContent_MakeAvailable(Catch2)

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(CTest) 
    include(Catch)

    add_executable(emulator_tests emulator.cpp)
    target_link_libraries(emulator_tests LINK_PUBLIC emulator)
    target_link_libraries(emulator_tests PRIVATE Catch2::Catch2WithMain)
    
    add_executable(cpu_tests cpu.cpp opcodes_table_addressing_modes.cpp opcodes_table_ops.cpp)
    target_link_libraries(cpu_tests LINK_PUBLIC emulator)
    target_link_libraries(cpu_tests PRIVATE Catch2::Catch2WithMain)

    add_executable(memory_tests nes_cpu_memory_accessor.cpp raw_memory_accessor.cpp)
    target_link_libraries(memory_tests LINK_PUBLIC emulator)
    target_link_libraries(memory_tests PRIVATE Catch2::Catch2WithMain)

    add_executable(rom_tests rom_tests.cpp)
    target_link_libraries(rom_tests LINK_PUBLIC emulator)
    target_link_libraries(rom_tests PRIVATE Catch2::Catch2WithMain)

    add_executable(ppu_tests ppu.cpp)
    target_link_libraries(ppu_tests LINK_PUBLIC emulator)
    target_link_libraries(ppu_tests PRIVATE Catch2::Catch2WithMain)

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    catch_discover_tests(emulator_tests)
    catch_discover_tests(cpu_tests)
    catch_discover_tests(memory_tests)
endif()