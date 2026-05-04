#ifndef TILE_CATCHER_H_
#define TILE_CATCHER_H_

#include <sstream>
#include <curl/curl.h>
#include <vector>
#include <future>
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

#include <stb_image.h>
#include <GL/glew.h>
#include "backends/imgui_impl_opengl3.h"


typedef struct tiles_s{
    int tile_size = 256;
    int x, y, z; // tile name
    int     width{tile_size};
    int     height{tile_size};
    int     channels{};
    bool    is_loaded;

    std::vector<unsigned char>  _rawBlob;
    unsigned char               *pixMap;
    GLuint _id{0};
} tiles_t;

double lon2x(const double lon, int z);
double lat2y(const double lat, int z);
double x2lon(const double x, int z);
double y2lat(const double y, int z);


#endif