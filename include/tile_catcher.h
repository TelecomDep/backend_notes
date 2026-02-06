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

void glLoad();

void stbLoad();

std::string makeUrl(int z, int x, int y);

size_t onPullResponse(void *data, size_t size, size_t nmemb, void *userp);

bool receiveTile(int z, int x, int y, std::vector<std::byte> &blob);

std::vector<std::byte> tileRequest(int z, int x, int y);
