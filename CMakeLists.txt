cmake_minimum_required(VERSION 3.10)
project(smelyzajko-gui)

set(CMAKE_CXX_STANDARD 17)

# ImGui source files
file(GLOB IMGUI_SRC libs/imgui/*.cpp libs/imgui/backends/imgui_impl_glfw.cpp libs/imgui/backends/imgui_impl_opengl3.cpp)
file(GLOB IMGUI_HEADERS libs/imgui/*.h libs/imgui/backends/imgui_impl_glfw.h libs/imgui/backends/imgui_impl_opengl3.h)

# Automatically collect all source files in src/
file(GLOB PROJECT_SOURCES src/*/*.cpp src/*.cpp TestModules/*.cpp)

# Add ImGui as a library
add_library(imgui ${IMGUI_SRC} ${IMGUI_HEADERS} ${PROJECT_SOURCES})

# Include directories
include_directories(libs/imgui libs/imgui/backends ${CMAKE_SOURCE_DIR} libs/ImGuiFileDialog libs/yaml-cpp/include)

add_subdirectory(libs/yaml-cpp)

# Create the main executable
add_executable(smelyzajko-gui libs/ImGuiFileDialog/ImGuiFileDialog.cpp main.cpp)

# Link the ImGui, OpenGL, and GLFW libraries
target_link_libraries(smelyzajko-gui PRIVATE imgui glfw GL yaml-cpp)
target_include_directories(smelyzajko-gui PRIVATE ${CMAKE_SOURCE_DIR}/libs/ImGuiFileDialog)


