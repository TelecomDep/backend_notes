
#include <sstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <future>
#include <memory>
#include <math.h>
#include <numbers>
#include <algorithm>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stb_image.h>
#include "stb_image_write.h"
#include <curl/curl.h>

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "imgui.h"
#include "implot.h"


struct Color {
    int r, g, b;
};

// Calculates the color at a specific 'ratio' (0.0 to 1.0)
Color gradientColor(Color c1, Color c2, double ratio) {
    return {
        static_cast<int>(c1.r + (c2.r - c1.r) * ratio),
        static_cast<int>(c1.g + (c2.g - c1.g) * ratio),
        static_cast<int>(c1.b + (c2.b - c1.b) * ratio)
    };
}


int main() {
    Color start = {255, 0, 0}; // Red
    Color end = {0, 255, 0};   // Green
    int steps = 100;

    float radius = 20;
    float lats[5] = { 55.008430, 55.008023, 55.007919, 55.007987, 55.008341 };
    float lons[5] = {82.944442, 82.944005, 82.944424, 82.945125, 82.945170};

    float x_pixels[5] = { 10, 40, 100, 120, 200 };
    float y_pixels[5] = {10, 50, 200, 134, 55};
    float value[5] = { 80, 5, 10, 95, 59 };

    int channels = 4; // RGB
    int h = 256;
    int w = 256;
    std::vector<unsigned char> image(w * h * channels);
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
        float summ_weight = 0;
        float summ_dmn = 0.001;
        for (int i = 0; i < 5; i++)
        {
            // Calculate distance per each experimental point
            float d = sqrt(pow(x - x_pixels[i], 2) + pow(y - y_pixels[i], 2));
            summ_dmn += float(1 / pow(d, 1));
            summ_weight += value[i] / d;
        }
        float weight = summ_weight / summ_dmn;

        double ratio = (double)weight / (steps - 1);
        Color current = gradientColor(start, end, ratio);
        
        int index = (y * w + x) * channels;
        image[index + 0] = current.r; // Red gradient
        image[index + 1] = current.g;   // Green gradient
        image[index + 2] = current.b;   // Blue constant
        image[index + 3] = 255;   // Alfa constant
        }
    }
    stbi_write_png("gradient.png", w, h, channels, image.data(), w * channels);

    unsigned char *pixMap = stbi_load("gradient.png", &w, &h, &channels, STBI_rgb_alpha);
}