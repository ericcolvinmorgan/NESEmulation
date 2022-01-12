
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2")
    set(SDL2_LIBRARIES "-s USE_SDL=2")
endif()

add_executable(wasm main.cpp)
target_link_libraries (wasm LINK_PUBLIC emulator ${SDL2_LIBRARIES})