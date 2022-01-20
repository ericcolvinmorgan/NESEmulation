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

    add_executable(test cpu.cpp emulator.cpp opcodes_table.cpp raw_memory_accessor.cpp)
    target_link_libraries(test LINK_PUBLIC emulator)
    target_link_libraries(test PRIVATE Catch2::Catch2WithMain)

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    catch_discover_tests(test)
endif()