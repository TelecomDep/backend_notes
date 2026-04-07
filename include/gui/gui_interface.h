#ifndef GUI_INTERFACE_H_
#define GUI_INTERFACE_H_

#include "tile_catcher.h"


typedef struct gui_runner_s{
    bool is_running;
    std::vector<tiles_t>    tiles;      // Работа с тайлами
} gui_runner_t;

void run_gui(gui_runner_t *gui_runner);

#endif