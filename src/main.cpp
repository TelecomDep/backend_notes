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
    global_runner.gui_runner._blankTile._rawBlob.resize(256 * 256 * 4);
    int RGBA_SZ = 4;
    std::array<uint8_t, 4> Snow = {100, 100, 150, 100};
    global_runner.gui_runner._blankTile._rawBlob.shrink_to_fit();
    for (size_t i = 0; i != global_runner.gui_runner._blankTile._rawBlob.size(); i = i + 4) {
        global_runner.gui_runner._blankTile._rawBlob[i] = (Snow[0]);
        global_runner.gui_runner._blankTile._rawBlob[i + 1] = (Snow[1]);
        global_runner.gui_runner._blankTile._rawBlob[i + 2] = (Snow[2]);
        global_runner.gui_runner._blankTile._rawBlob[i + 3] = (Snow[3]);
    }
    stbLoad(&global_runner.gui_runner._blankTile);
    glLoad(&global_runner.gui_runner._blankTile);
    
    
    std::thread gui_thread(run_gui, &global_runner.gui_runner);

    gui_thread.join();
    return 0;
}