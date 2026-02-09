cmake_minimum_required(VERSION 3.14)
project(backend_notes)

set(CMAKE_CXX_STANDARD 20)

set(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/third_party)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(EXAMPLES_DIR ${PROJECT_SOURCE_DIR}/examples)

# SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# OpenGL/GLEW
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include(FindPkgConfig)
pkg_check_modules(STB REQUIRED stb)

include_directories(${INCLUDE_DIR})


# Проверка на дурака, установили ли библиотеки 
if(NOT EXISTS "${THIRD_PARTY_DIR}/imgui/")
    message(FATAL_ERROR "ImGui submodule not found. Did you run 'git submodule update --init --recursive' or 'git submodule update --remote --recursive'?")
endif()
if(NOT EXISTS "${THIRD_PARTY_DIR}/implot/")
    message(FATAL_ERROR "ImPlot submodule not found. Did you run 'git submodule update --init --recursive' or 'git submodule update --remote --recursive'?")
endif()


# ImGui
add_library(imgui STATIC
    ${THIRD_PARTY_DIR}/imgui/imgui.cpp
    ${THIRD_PARTY_DIR}/imgui/imgui_draw.cpp
    ${THIRD_PARTY_DIR}/imgui/imgui_demo.cpp
    ${THIRD_PARTY_DIR}/imgui/imgui_tables.cpp
    ${THIRD_PARTY_DIR}/imgui/imgui_widgets.cpp
    ${THIRD_PARTY_DIR}/imgui/backends/imgui_impl_sdl2.cpp
    ${THIRD_PARTY_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${THIRD_PARTY_DIR}/imgui)
target_compile_options(imgui PRIVATE -fPIC)

# ImPlot
add_library(implot STATIC
    ${THIRD_PARTY_DIR}/implot/implot.cpp
    ${THIRD_PARTY_DIR}/implot/implot_demo.cpp
    ${THIRD_PARTY_DIR}/implot/implot_items.cpp
)
target_include_directories(implot PUBLIC ${THIRD_PARTY_DIR}/implot)
target_link_libraries(implot PRIVATE imgui)
target_compile_options(implot PRIVATE -fPIC)


add_executable(main ${SRC_DIR}/main.cpp)
target_link_libraries(main PRIVATE imgui implot ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

add_executable(tile ${EXAMPLES_DIR}/osm_tiles/tile_catcher.cpp)
target_link_libraries(tile PRIVATE imgui implot curl ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${STB_LIBRARIES})
target_include_directories(tile PRIVATE ${STB_INCLUDE_DIRS})

add_executable(sdr_samples_update ${EXAMPLES_DIR}/samples_update_thread/test_samples_update.cpp)
target_link_libraries(sdr_samples_update PRIVATE imgui implot  ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})