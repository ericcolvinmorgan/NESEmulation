find_package(Catch2 REQUIRED)
include(CTest) 
include(Catch)

add_executable(test test.cpp)
target_link_libraries(test LINK_PUBLIC emulator)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
catch_discover_tests(test)