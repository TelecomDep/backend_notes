#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "imgui.h"
#include "implot.h"
#include "imgui_internal.h"

#include "gui_interface.h"
#include "defs.h"

int main(int argc, char *argv[]) {

    my_global_t global_runner;
    
    std::thread gui_thread(run_gui, &global_runner.gui_runner);

    gui_thread.join();
    return 0;
}