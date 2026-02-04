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

template <typename T>
inline T RandomRange(T min, T max) {
    T scale = rand() / (T) RAND_MAX;
    return min + scale * ( max - min );
}



void run_gui(){
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

    bool running = true;
    // auto last_frame_time = std::chrono::steady_clock::now();
    while (running) {

        // Обработка event'ов (inputs, window resize, mouse moving, etc.)
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            std::cout << "Processing some event: "<< event.type << " timestamp: " << event.motion.timestamp << std::endl;
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_None);

        //1. 
        {
            ImGui::SetNextWindowSize(ImVec2(686,416));
            static int counter = 0;

            ImGui::Begin("Hello, world!");
            if (ImGui::Button("Button"))
                counter++;
            ImGui::Text("counter = %d", counter);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::Text("Window size: %lfx%lf", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
            ImGui::End();
        }
        {
            static int counter = 0;
            if (ImGui::Button("Button")){
                counter++;
                // можно что угодно добавить при нажатии на кнопку
            }                        
            ImGui::Text("counter = %d", counter);

            ImGui::Button("change me", ImVec2(91,59));
        }

        // // Default color style
        // {
        //     ImGui::Begin("Color theme");

        //     static int style_idx = 0;

        //     if (ImGui::Button("Light"))
        //         style_idx = 1;
        //     if (ImGui::Button("Dark"))
        //         style_idx = 0;
        //     if (ImGui::Button("Classic"))
        //         style_idx = 2;

        //     switch (style_idx)
        //     {
        //         case 0: ImGui::StyleColorsDark(); break;
        //         case 1: ImGui::StyleColorsLight(); break;
        //         case 2: ImGui::StyleColorsClassic(); break;
        //     }

        //     ImGui::End();
        // }
           
        // ImGui::ShowDemoWindow();

        ImGui::Begin("Simple Plot");
        static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
        ImGui::PlotLines("Frame Times", arr, IM_COUNTOF(arr));
        ImGui::End();
        // ImPlot::ShowDemoWindow();
        // ImGui::Begin("Mouse Position");
        // ImVec2 mouse = ImGui::GetMousePos();
        // ImGui::Text("Mouse position: x = %f, y = %f", mouse.x, mouse.y);
        // ImGui::End();

        // {
        //     ImGui::Begin("My Plot!");
        //     int N = 500;
        //     double data_x[N];
        //     double data_y[N];
        //     for (int i = 0; i < N; i++){
        //         data_x[i] = RandomRange(400.0,450.0);
        //         data_y[i] = RandomRange(100.0,150.0);
        //     }
        //     if(ImPlot::BeginPlot("##Scrolling"))
        //     {
        //         ImPlot::PlotScatter("Mouse X", &data_x[0], &data_y[0], N, 0, 0, 2 * sizeof(float));
        //         ImPlot::EndPlot();
        //     }
        
        //     ImGui::End();
        // }


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


int main(int argc, char *argv[]) {

    std::thread gui_thread(run_gui);
    gui_thread.join();
    return 0;
}

        // Cool plots
        // {
        //     ImGui::Begin("Hello, Plots!");
        //     static ScrollingBuffer sdata1, sdata2;
        //     static RollingBuffer rdata1, rdata2;
        //     // Add points to the buffers every 0.02 seconds
        //     static float t = 0, last_t = 0.0f;
        //     if (t == 0 || t - last_t >= 0.001f)
        //     {
        //         sdata1.AddPoint(t, mouse.x * 0.0005f);
        //         rdata1.AddPoint(t, mouse.x * 0.0005f);
        //         sdata2.AddPoint(t, mouse.y * 0.0005f);
        //         rdata2.AddPoint(t, mouse.y * 0.0005f);
        //         last_t = t;
        //     }
        //     t += ImGui::GetIO().DeltaTime;

        //     static float history = 10.0f;
        //     ImGui::SliderFloat("History",&history,1,30,"%.1f s");
        //     rdata1.Span = history;
        //     rdata2.Span = history;

        //     static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

        //     if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1,ImGui::GetTextLineHeight()*10))) {
        //         ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
        //         ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
        //         ImPlot::SetupAxisLimits(ImAxis_Y1,0,1);
        //         ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
        //         ImPlot::PlotShaded("Mouse X", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), -INFINITY, 0, sdata1.Offset, 2 * sizeof(float));
        //         ImPlot::PlotLine("Mouse Y", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2*sizeof(float));
        //         ImPlot::EndPlot();
        //     }
        //     if (ImPlot::BeginPlot("##Rolling", ImVec2(-1,ImGui::GetTextLineHeight()*10))) {
        //         ImPlot::SetupAxes(nullptr, nullptr, flags, flags);
        //         ImPlot::SetupAxisLimits(ImAxis_X1,0,history, ImGuiCond_Always);
        //         ImPlot::SetupAxisLimits(ImAxis_Y1,0,1);
        //         ImPlot::PlotLine("Mouse X", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 0, 2 * sizeof(float));
        //         ImPlot::PlotLine("Mouse Y", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 0, 2 * sizeof(float));
        //         ImPlot::EndPlot();
        //     }
            
        //     ImGui::End();
        // }