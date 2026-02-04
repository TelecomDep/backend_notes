#include <sstream>
#include <curl/curl.h>
#include <vector>
#include <future>
#include <memory>
#include <iostream>


// struct FutureData {
//     std::shared_ptr<ITile> tile;
// };


std::string makeUrl(int z, int x, int y) {
  std::ostringstream urlmaker;
  urlmaker << "https://a.tile.openstreetmap.org";
  urlmaker << '/' << z << '/' << x << '/' << y << ".png";
  return urlmaker.str();
}

size_t onPullResponse(void *data, size_t size, size_t nmemb,
                                     void *userp) {
  size_t realsize{size * nmemb};
  auto &blob{*static_cast<std::vector<std::byte> *>(userp)};
  auto const *const dataptr{static_cast<std::byte *>(data)};
  blob.insert(blob.cend(), dataptr, dataptr + realsize);
  return realsize;
}

bool receiveTile(int z, int x, int y,
                                std::vector<std::byte> &blob) {
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

void onHandleRequest(int z, int x, int y) {
  std::vector<std::byte> blob;
  if (receiveTile(z, x, y, blob)) {
      std::cout << "tile is received" << std::endl;
      // тут в blob лежат байтики после получения тайлов
  } else {
    std::cout << "tile is not received" << std::endl;
    // тут Dummy байтики
  }
//   return data;
}

int main(){

  onHandleRequest(10, 20, 13);
  return 0;
}