// FROM https://developer.mozilla.org/en-US/docs/WebAssembly/C_to_wasm
// AND https://emscripten.org/docs/getting_started/Tutorial.html

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <stdio.h>
#include <iostream>
#include <random>
#include <SDL2/SDL.h>
#include "../../include/emulator/cpu.h"
#include "../../include/emulator/6502/emulator.h"
#include "../../include/emulator/opcodes_table.h"
#include "../../include/emulator/raw_memory_accessor.h"
#include "../../include/interface/demo_controller.h"
#include "../../include/interface/sdl_video.h"

#include <fstream>
#include <ios>

const int kFPS = 60;
std::random_device r;
std::default_random_engine generator(r());
CPU *cpu = nullptr;
OpCodesInterface *cpu_opcodes = nullptr;
Emulator *emulator = nullptr;
MemoryAccessorInterface *memory = nullptr;
VideoInterface *content_screen = nullptr;
ControllerInterface *controller = nullptr;
bool request_exit = false;

static int SDLCALL HandleExit(void *userdata, SDL_Event * event)
{
    if (event->type == SDL_QUIT) {
        bool *exit = (bool*) userdata;
        *exit = true;
    }
    return 1;  // let all events be added to the queue since we always return 1.
}

void RenderFrame()
{
    // Set Random Number
    std::uniform_int_distribution<uint8_t> uniform_dist(1, 255);
    memory->WriteMemory(0x00fe, uniform_dist(generator));
    controller->WriteInput(0x00ff);
    emulator->AdvanceFrame();
    content_screen->RenderFrame();
}

#ifdef __EMSCRIPTEN__
void RunEmulator()
{
    emscripten_set_main_loop(RenderFrame, kFPS, 1);
}
#else
void RunEmulator()
{
    SDL_SetEventFilter(HandleExit, &request_exit);
    while (!request_exit)
	{
        Uint64 start = SDL_GetPerformanceCounter();
		RenderFrame();
        Uint64 end = SDL_GetPerformanceCounter();
        float elapsed_milliseconds = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.f;

        //Cap FPS to kFPS
        float delay = floor((1000.f / kFPS) - elapsed_milliseconds);
        if(delay > 0.f)
            SDL_Delay(delay);
	}
}
#endif

int main(int argc, char** argv)
{
    #ifdef __EMSCRIPTEN__
        const char* file_path = "snake.bin";
    #else
    if(argc != 2)
    {
        std::cout << "Please provide a file path argument.\n";
        return 0;
    }
    const char* file_path = argv[1];
    #endif

    std::ifstream inputFile(file_path, std::ios::binary);
    if(!inputFile.is_open())
    {
        std::cout << "The provided file is not accessible.\n";
        return 0;
    }

    // Determine the file length
    inputFile.seekg(0, std::ios_base::end);
    std::size_t rom_size = inputFile.tellg();
    inputFile.seekg(0, std::ios_base::beg);

    //Read contents to memory and close file.
    Byte *rom_data = new Byte[rom_size];
    inputFile.read((char *)rom_data, rom_size);
    inputFile.close();

    // Initialize
    memory = new RawMemoryAccessor();

    memory->WriteMemory(0x0600, rom_data, rom_size);
    delete[] rom_data;

    memory->WriteMemory(kReset, (Word)0x0600);
    
    content_screen = new SDLVideo(memory);
    content_screen->InitVideo();

    controller = new DemoController(memory);
    controller->InitController();
    
    cpu = new CPU({.sp = 0xFF}, memory);
    cpu_opcodes = new OpCodesTable();
    cpu->Reset();
    emulator = new Emulator(cpu, cpu_opcodes, 300);

    RunEmulator();
    
    delete controller;
    controller = nullptr;

    delete content_screen;
    content_screen = nullptr;

    delete emulator;
    emulator = nullptr;

    delete cpu_opcodes;
    cpu_opcodes = nullptr;

    delete cpu;
    cpu = nullptr;

    delete memory;
    memory = nullptr;


    return 0;
}