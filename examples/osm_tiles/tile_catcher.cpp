#include <sstream>
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <future>
#include <memory>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stb_image.h>
#include <curl/curl.h>

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "imgui.h"
#include "implot.h"

#include "tile_catcher.h"



// struct FutureData {
//     std::shared_ptr<ITile> tile;
// };

int _width{256}, _height{256}, _channels{};
std::vector<std::byte> _rawBlob;
std::vector<std::byte> _rgbaBlob;
GLuint _id{0};
bool loaded = false;

void glLoad(){
  glGenTextures(1, &_id);
  glBindTexture(GL_TEXTURE_2D, _id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, _rgbaBlob.data());
}

void stbLoad() {
  stbi_set_flip_vertically_on_load(false);
  const auto ptr{
      stbi_load_from_memory(reinterpret_cast<stbi_uc const *>(_rawBlob.data()),
                            static_cast<int>(_rawBlob.size()), &_width, &_height,
                            &_channels, STBI_rgb_alpha)};
  if (ptr) {
    const size_t nbytes{size_t(_width * _height * STBI_rgb_alpha)};
    _rgbaBlob.resize(nbytes);
    _rgbaBlob.shrink_to_fit();
    const auto byteptr{reinterpret_cast<std::byte *>(ptr)};
    _rgbaBlob.insert(_rgbaBlob.begin(), byteptr, byteptr + nbytes);
    stbi_image_free(ptr);
  }
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
  auto &blob{*static_cast<std::vector<std::byte> *>(userp)};
  auto const *const dataptr{static_cast<std::byte *>(data)};
  blob.insert(blob.cend(), dataptr, dataptr + realsize);
  std::cout << "tile size = " << realsize << std::endl;
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
  loaded = true;
  return ok;
}

std::vector<std::byte> tileRequest(int z, int x, int y) {
  std::vector<std::byte> blob;
  if (receiveTile(z, x, y, blob)) {
      std::cout << "tile is received" << std::endl;
      // тут в blob лежат байтики после получения тайлов
  } else {
    std::cout << "tile is not received" << std::endl;
    // тут Dummy байтики
  }
   return blob;
}

int main(){

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

        


        //18/232798/103246.png
        ImPlot::BeginPlot("##ImOsmMapPlot");
        ImVec2 _uv0{0, 1}, _uv1{1, 0};
        ImVec4 _tint{1, 1, 1, 1};
        ImVec2 bmin{0, 0};
        ImVec2 bmax{256, 256};
        if(!loaded){
          _rawBlob = tileRequest(2, 2, 1);
          stbLoad();
          glLoad();
          std::cout << "raw blob size = " << _rawBlob.size() << std::endl;
          std::cout << "rgb blob size = " << _rgbaBlob.size() << std::endl;
        }
        if (loaded)
        {
          ImPlot::PlotImage("##", _id, bmin, bmax, _uv0, _uv1, _tint);
        }

        ImPlot::EndPlot();

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
  return 0;
}