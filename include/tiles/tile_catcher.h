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
    std::array<int, 3> zxy{0, 0, 0};
    int     tile_size = 256;
    int     width{tile_size};
    int     height{tile_size};
    int     channels{};
    bool    is_loaded = false;
    std::future<bool> async_result;
    CURLcode code;

    std::vector<unsigned char>  _rawBlob;
    unsigned char               *pixMap;
    mutable GLuint _id = 0;
    bool operator == (const tiles_s &other) { 
        return this->zxy == other.zxy;
    }
} tiles_t;

double lon2x(const double lon, int z);
double lat2y(const double lat, int z);
double x2lon(const double x, int z);
double y2lat(const double y, int z);

tiles_t tileRequest(int z, int x, int y);
void glLoad(tiles_t *tile);
void stbLoad(tiles_t *tile);

#endif