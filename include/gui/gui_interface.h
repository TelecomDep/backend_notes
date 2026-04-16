#ifndef GUI_INTERFACE_H_
#define GUI_INTERFACE_H_

#include <list>
#include "tile_catcher.h"


typedef struct gui_runner_s{
    bool is_running;
    int tile_size = 256;
    int zoom = 13;
    int tiles_count;
    std::vector<tiles_t> tiles;
    tiles_t _blankTile;
} gui_runner_t;


// Основной поток работы ImGUI
void run_gui(gui_runner_t *gui_runner);

// Функции отображения разных элементов
void main_menu(gui_runner_t *gui_runner);
void properties_window(gui_runner_t *gui_runner);
void main_window(gui_runner_t *gui_runner);
void bottom_status_bar(gui_runner_t *gui_runner);

void plot_osm_map(gui_runner_t *gui_runner);
GLuint tileAt(int z, int x, int y, gui_runner_t *gui_runner);

#endif