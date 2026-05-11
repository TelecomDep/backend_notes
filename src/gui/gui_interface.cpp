#include "gui_interface.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <algorithm>
#include <future>
#include <memory>
#include <sstream>

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "imgui.h"
#include "implot.h"
#include "imgui_internal.h"

#include "tile_catcher.h"




void properties_window(gui_runner_t *gui_runner)
{
    static int counter = 0;
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::Begin("Properties");
    if (ImGui::Button("Button"))
        counter++;
    ImGui::Text("counter = %d", counter);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text("Window size: %lfx%lf", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    ImGui::End();
}


GLuint tileAt(int z, int x, int y, gui_runner_t *gui_runner){
    
    const auto& it = std::find(gui_runner->tiles.begin(), gui_runner->tiles.end(),
                          tiles_t{std::array<int, 3>{z, x, y}});
    bool is_loaded = false;
    int index = 0;
    for (int i = 0; i < gui_runner->tiles.size(); i++)
    {
        if(gui_runner->tiles[i].zxy[0] == z && gui_runner->tiles[i].zxy[1] == x && gui_runner->tiles[i].zxy[2] == y){
            is_loaded = true;
            index = i;
            break;
        }
    }
    if(is_loaded){
        if(gui_runner->tiles[index].is_loaded){
            if(gui_runner->tiles[index]._id == 0){
                stbLoad(&gui_runner->tiles[index]);
                glLoad(&gui_runner->tiles[index]);
            }
            return gui_runner->tiles[index]._id;
        }
    } else {
         gui_runner->tiles.push_back(tileRequest(z, x, y));
    }
    return 0;
}

void plot_osm_map(gui_runner_t *gui_runner)
{
    // Получаем размер окна с графиком в пикселях
    ImVec2 win_size = ImGui::GetWindowSize();

    // Считаем сколько картинок необходимо, чтобы заполнить весь виджет тайлами.
    int nof_x_tiles = std::floor(win_size.x / gui_runner->tile_size);
    int nof_y_tiles = std::floor(win_size.y / gui_runner->tile_size);

    // Выбираем центр на карте, чтобы от нее начинать считать тайлы
    double lat_center = 55.013266;
    double lon_center = 82.950782;
    double delta_lla = 0.1;
    int zoom = gui_runner->zoom;

    // Выставляем границы графика по осям X, Y.
    ImPlot::SetNextAxesLimits(  lon_center - delta_lla, lon_center + delta_lla, 
                                lat_center - delta_lla, lat_center + delta_lla, ImPlotCond_Once);
    ImPlot::BeginPlot("##ImOsmMapPlot", {-1, -1}); // size = {-1, -1} - растянет на весь виджет
    ImPlotRect axisLimits = ImPlot::GetPlotLimits();

    // Находим границы номеров тайлов.
    int minX = lon2x(axisLimits.X.Min, zoom);
    int maxX = lon2x(axisLimits.X.Max, zoom);
    int minY = lat2y(axisLimits.Y.Max, zoom); // Здесь намерено Min Max поменяли местами. См. нумерацию тайлов.
    int maxY = lat2y(axisLimits.Y.Min, zoom); // Здесь намерено Min Max поменяли местами. См. нумерацию тайлов.
    
    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
                ImVec2 uv0{0, 0};
                ImVec2 uv1{1, 1};
                ImVec4 tint{1, 1, 1, 1};
                ImPlotPoint bmin{x2lon(x, zoom), y2lat(y + 1, zoom)};
                ImPlotPoint bmax{x2lon(x + 1, zoom), y2lat(y, zoom)};
                ImPlot::PlotImage("##tile_", (ImTextureID)(intptr_t)tileAt(zoom, x, y, gui_runner), bmin, bmax, uv0, uv1, tint);
        }
    }

    ImPlot::EndPlot();
}

void main_window(gui_runner_t *gui_runner)
{
    ImGui::Begin("Main", nullptr, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginTabBar("Main")) {
        if (ImGui::BeginTabItem("Info")) {
            


            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Custom Map")) {

            plot_osm_map(gui_runner);

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}

void bottom_status_bar(gui_runner_t *gui_runner)
{
    if (ImGui::BeginViewportSideBar(    "##MainStatusBar", ImGui::GetMainViewport(), 
                                        ImGuiDir_Down, ImGui::GetFrameHeight(), 
                                        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | 
                                        ImGuiWindowFlags_MenuBar)) {
        if (ImGui::BeginMenuBar()) {
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            ImGui::Text("Frame Rate: %.3f [ms/frame] (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }
}

void main_menu(gui_runner_t *gui_runner)
{
    if(ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void run_gui(gui_runner_t *gui_runner)
{

    gui_runner->is_running = true;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    SDL_Window* window = SDL_CreateWindow(
        "Backend start", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Включить Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Включить Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Включить Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Включить Multi-Viewport / Platform Windows. Позволяет работать "окнам" вне основного окна. 

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");

    // auto last_frame_time = std::chrono::steady_clock::now();
    while (gui_runner->is_running) {

        // Обработка event'ов (inputs, window resize, mouse moving, etc.)
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // std::cout << "Processing some event: "<< event.type << " timestamp: " << event.motion.timestamp << std::endl;
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                gui_runner->is_running = false;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        // ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_None);
        // ImGui::DockSpaceOverViewport();
        // ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGuiID dockspace_id = ImGui::GetID("My Dockspace");
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Create settings
        if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr)
        {
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
            ImGuiID dock_id_left = 0;
            ImGuiID dock_id_main = dockspace_id;
            ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.20f, &dock_id_left, &dock_id_main);
            ImGuiID dock_id_left_top = 0;
            ImGuiID dock_id_left_bottom = 0;
            ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.50f, &dock_id_left_top, &dock_id_left_bottom);
            ImGui::DockBuilderDockWindow("Main", dock_id_main);
            ImGui::DockBuilderDockWindow("Properties", dock_id_left_top);
            ImGui::DockBuilderDockWindow("Scene", dock_id_left_bottom);
            ImGui::DockBuilderFinish(dockspace_id);
        }
        ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_PassthruCentralNode);

        
        main_menu(gui_runner);
        properties_window(gui_runner);
        main_window(gui_runner);
        bottom_status_bar(gui_runner);

        ImGui::Render();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}