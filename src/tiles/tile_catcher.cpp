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
#include <curl/curl.h>

#include "tile_catcher.h"

double _minLat{-85.0};
double _maxLat{+85.0};
double _minLon{-179.9};
double _maxLon{+179.9};
int MinZoom{0};
int MaxZoom{18};

double PI{std::numbers::pi_v<double>};
double PI2{std::numbers::pi_v<double> * 2.0};
double RAD{std::numbers::pi_v<double> / 180.0};
double DEG{180.0 / std::numbers::pi_v<double>};

int POW2[]{
    (1 << 0),  (1 << 1),  (1 << 2),  (1 << 3),  (1 << 4),  (1 << 5),  (1 << 6),
    (1 << 7),  (1 << 8),  (1 << 9),  (1 << 10), (1 << 11), (1 << 12), (1 << 13),
    (1 << 14), (1 << 15), (1 << 16), (1 << 17), (1 << 18)};


double lon2x(const double lon, int z = 0) {
  return (lon + 180.0) / 360.0 * double(POW2[z]);
}

double lat2y(const double lat, int z = 0) {
  return (1.0 - asinh(tan(lat * RAD)) / PI) / 2.0 * double(POW2[z]);
}

double x2lon(const double x, int z = 0) {
  return x / double(POW2[z]) * 360.0 - 180.0;
}

double y2lat(const double y, const int z = 0) {
  const double n{PI - PI2 * y / double(POW2[z])};
  return DEG * atan(0.5 * (exp(n) - exp(-n)));
}

void glLoad(tiles_t *tile){
  glGenTextures(1, &tile->_id);
  glBindTexture(GL_TEXTURE_2D, tile->_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tile->width, tile->height, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, tile->pixMap);
}

void stbLoad(tiles_t *tile) {
  tile->pixMap = stbi_load_from_memory(tile->_rawBlob.data(), tile->_rawBlob.size(), &tile->width, &tile->height, &tile->channels, STBI_rgb_alpha);
}


std::string makeUrl(int z, int x, int y) {
  std::ostringstream urlmaker;
  urlmaker << "https://a.tile.openstreetmap.org";
  urlmaker << '/' << z << '/' << x << '/' << y << ".png";
  return urlmaker.str();
}

size_t onPullResponse(void *data, size_t size, size_t nmemb,
                                     void *userp) {
  size_t realsize{size * nmemb};
  auto &blob{*static_cast<std::vector<unsigned char> *>(userp)};
  auto const *const dataptr{static_cast<unsigned char *>(data)};
  blob.insert(blob.cend(), dataptr, dataptr + realsize);
  std::cout << "tile size = " << realsize << std::endl;
  return realsize;
}

bool receiveTile(int z, int x, int y,
                                std::vector<unsigned char> &blob) {
  CURL *curl{curl_easy_init()};
  curl_easy_setopt(curl, CURLOPT_URL, makeUrl(z, x, y).c_str());
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl");
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&blob);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onPullResponse);
  const bool ok{curl_easy_perform(curl) == CURLE_OK};
  curl_easy_cleanup(curl);

  return ok;
}

bool tileRequest(int z, int x, int y, tiles_t *tile) {

  if (receiveTile(z, x, y, tile->_rawBlob)) {
      std::cout << "tile is received" << std::endl;
      tile->is_loaded = true;
      return true;
      // тут в blob лежат байтики после получения тайлов
  } else {
    return false;
    // тут Dummy байтики
  }
}