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
#include "backends/imgui_impl_opengl3.h"


typedef struct tiles_s{
    int     x, y, z; // tile name
    int     width{256};
    int     height{256};
    int     channels{};
    bool    is_loaded;

    std::vector<unsigned char>  _rawBlob;
    unsigned char               *pixMap;
    GLuint _id{0};
} tiles_t;




#endif