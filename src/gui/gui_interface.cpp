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


void plot_osm_map(gui_runner_t *gui_runner)
{
    // ImPlot::BeginPlot("##ImOsmMapPlot");

    // if(!loaded){
    //     std::cout << "min max X = " << _minX << " " << _maxX << std::endl;
    //     std::cout << "min max y = " << _minY << " " << _maxY << std::endl;
    // }
    // // Top-left of the texture
    // // Bottom-right of the texture
    // ImVec2 _uv0{0, 1}, _uv1{1, 0};
    // ImVec4 _tint{1, 1, 1, 1};
    // ImVec2 bmin{0, 0};
    // ImVec2 bmax{256, 256};
    // if(!loaded){

    //     std::cout << "min max X = " << _minX << " " << _maxX << std::endl;
    //     std::cout << "min max y = " << _minY << " " << _maxY << std::endl;
    //     _rawBlob = tileRequest(16, 47867, 20726);
    
    //     stbLoad();
    //     glLoad();
    // }
    // if (loaded)
    // {
    //     ImPlot::PlotImage("##", _id, bmin, bmax, _uv0, _uv1, _tint);
    // }

    // ImPlot::EndPlot();
}

void main_window(gui_runner_t *gui_runner)
{
    ImGui::Begin("Main", nullptr, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginTabBar("Main")) {
        if (ImGui::BeginTabItem("Info")) {
            


            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Custom Map")) {
            
            
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