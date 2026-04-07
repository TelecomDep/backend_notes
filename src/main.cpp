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