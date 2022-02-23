
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2") 
    set(SDL2_LIBRARIES "-s USE_SDL=2")
    add_executable(6502 sdl_video.cpp demo_controller.cpp 6502_main.cpp)
    target_link_libraries (6502 LINK_PUBLIC 6502_emulator ${SDL2_LIBRARIES})
    set_target_properties(6502 PROPERTIES LINK_FLAGS "--preload-file ../../../roms/6502/colors.bin@colors.bin --preload-file ../../../roms/6502/snake.bin@snake.bin")

    add_executable(nes nes_sdl_video.cpp demo_controller.cpp nes_controller.cpp keyboard_interface.cpp nes_main.cpp)
    target_link_libraries (nes LINK_PUBLIC nes_emulator ${SDL2_LIBRARIES})
    set_target_properties(nes PROPERTIES LINK_FLAGS "--preload-file ../../../roms/nes-test/nestest.nes@nestest.nes")

else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") 
    # set(SDL2_INCLUDE_DIR /usr/include/SDL2)
    # include_directories(${SDL2_INCLUDE_DIR})
    # set(SDL2_LIBRARIES "-s USE_SDL=2")
    add_executable(6502 sdl_video.cpp demo_controller.cpp 6502_main.cpp)
    add_executable(nes nes_sdl_video.cpp demo_controller.cpp nes_controller.cpp keyboard_interface.cpp nes_main.cpp)

    INCLUDE(FindPkgConfig)

    PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
    target_link_libraries (6502 LINK_PUBLIC 6502_emulator ${SDL2_LIBRARIES})
    target_link_libraries (nes LINK_PUBLIC nes_emulator ${SDL2_LIBRARIES})
endif()