cmake_minimum_required(VERSION 3.10)
project(smelyzajko-gui)

set(CMAKE_CXX_STANDARD 17)

# ImGui source files
file(GLOB IMGUI_SRC imgui/*.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)
file(GLOB IMGUI_HEADERS imgui/*.h imgui/backends/imgui_impl_glfw.h imgui/backends/imgui_impl_opengl3.h)

# Add ImGui as a library
add_library(imgui ${IMGUI_SRC} ${IMGUI_HEADERS})

# Include directories
include_directories(imgui imgui/backends)

# Create the main executable
add_executable(smelyzajko-gui main.cpp)

# Link the ImGui, OpenGL, and GLFW libraries
target_link_libraries(smelyzajko-gui PRIVATE imgui glfw GL)