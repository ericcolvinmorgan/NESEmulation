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
#include "../../include/emulator/emulator.h"
#include "../../include/emulator/nes_cpu_memory_accessor.h"
#include "../../include/emulator/nes_ppu_memory_accessor.h"
#include "../../include/emulator/opcodes_table.h"
#include "../../include/emulator/ppu.h"
#include "../../include/interface/demo_controller.h"
#include "../../include/interface/nes_controller.h"
#include "../../include/interface/nes_sdl_video.h"
#include "../../include/interface/keyboard_interface.h"

#include <fstream>
#include <ios>

const int kFPS = 60;
std::random_device r;
std::default_random_engine generator(r());
CPU *cpu = nullptr;
PPU *ppu = nullptr;
OpCodesInterface *cpu_opcodes = nullptr;
Emulator *emulator = nullptr;
MemoryAccessorInterface *cpu_memory = nullptr;
MemoryAccessorInterface *ppu_memory = nullptr;
VideoInterface *content_screen = nullptr;
NESController *controller = nullptr;
bool request_exit = false;
bool toggle_logging = false;

static int SDLCALL HandleEvents(void *userdata, SDL_Event *event)
{
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_t)
    {
        toggle_logging = !toggle_logging;

        if (emulator != nullptr)
            emulator->EnableLogging(toggle_logging);
    }

    if (event->type == SDL_QUIT)
    {
        request_exit = true;
    }

    return 1; // let all events be added to the queue since we always return 1.
}

void RenderFrame()
{
    emulator->AdvanceFrame();
    controller->PollInputIfStrobing();
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
    SDL_SetEventFilter(HandleEvents, NULL);
    while (!request_exit)
    {
        Uint64 start = SDL_GetPerformanceCounter();
        RenderFrame();
        Uint64 end = SDL_GetPerformanceCounter();
        float elapsed_milliseconds = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.f;

        // Cap FPS to kFPS
        float delay = floor((1000.f / kFPS) - elapsed_milliseconds);
        if (delay > 0.f)
            SDL_Delay(delay);
    }
}
#endif

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Please provide a file path argument.\n";
        return 0;
    }

    std::ifstream input_file(argv[1], std::ios::binary);
    if (!input_file.is_open())
    {
        std::cout << "The provided file is not accessible.\n";
        return 0;
    }

    // Determine the file length
    input_file.seekg(0, std::ios_base::end);
    std::size_t rom_size = input_file.tellg();
    input_file.seekg(0, std::ios_base::beg);

    // Read contents to memory and close file.
    Byte *rom_data = new Byte[rom_size];
    input_file.read((char *)rom_data, rom_size);
    input_file.close();

    // Attempt to read NES file format header.
    if (rom_data[0] != 'N' || rom_data[1] != 'E' || rom_data[2] != 'S' || rom_data[3] != 0x1a)
    {
        std::cout << "The provided file is not a valid NES ROM.\n";
        delete[] rom_data;
        return 0;
    }

    // Initialize
    cpu_memory = new NESCPUMemoryAccessor();

    if(rom_data[4] == 0x01)
    {
        cpu_memory->WriteMemory(0x8000, rom_data + 16, 0x4000);
        cpu_memory->WriteMemory(0xc000, rom_data + 16, 0x4000);
        ppu_memory = new NESPPUMemoryAccessor();
        ppu_memory->WriteMemory(0x0000, rom_data + 16 + 16384, 0x2000);
        delete[] rom_data;
    }
    else if(rom_data[4] == 0x02)
    {
        cpu_memory->WriteMemory(0x8000, rom_data + 16, 0x4000);
        cpu_memory->WriteMemory(0xc000, rom_data + 16 + 0x4000, 0x4000);
        ppu_memory = new NESPPUMemoryAccessor();
        ppu_memory->WriteMemory(0x0000, rom_data + 16 + 0x8000, 0x2000);
        delete[] rom_data;
    }
    else
    {
        std::cout << "Invalid mapper 0 configuration.\n";
        delete cpu_memory;
        cpu_memory = nullptr;
        delete[] rom_data;
        return 0;
    }

    controller = new NESController(cpu_memory, new KeyboardInterface());

    cpu = new CPU({.sp = 0xFF, .pc = 0xc000}, cpu_memory);
    cpu_opcodes = new OpCodesTable();
    cpu->Reset();

    ppu = new PPU(ppu_memory, cpu_memory);

    content_screen = new NESSDLVideo(ppu, ppu_memory);
    content_screen->InitVideo();

    emulator = new Emulator(ppu, cpu, cpu_opcodes);

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

    delete cpu_memory;
    cpu_memory = nullptr;

    delete ppu_memory;
    ppu_memory = nullptr;

    return 0;
}