#cmake_minimum_required(VERSION 3.10)
#project(smelyzajko-gui)
#
#set(CMAKE_CXX_STANDARD 17)
#
## ImGui source files
#file(GLOB IMGUI_SRC imgui/*.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)
#file(GLOB IMGUI_HEADERS imgui/*.h imgui/backends/imgui_impl_glfw.h imgui/backends/imgui_impl_opengl3.h)
#
## Automatically collect all source files in src/
#file(GLOB PROJECT_SOURCES src/*.cpp)
#
## Add ImGui as a library
#add_library(imgui ${IMGUI_SRC} ${IMGUI_HEADERS} ${PROJECT_SOURCES})
#
## Include directories
#include_directories(imgui imgui/backends)
#
## Create the main executable
#add_executable(smelyzajko-gui main.cpp)
#
## Link the ImGui, OpenGL, and GLFW libraries
#target_link_libraries(smelyzajko-gui PRIVATE imgui glfw GL)



cmake_minimum_required(VERSION 3.10)
project(smelyzajko-gui)

set(CMAKE_CXX_STANDARD 17)


file(GLOB IMGUI_SRC imgui/*.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)
file(GLOB IMGUI_HEADERS imgui/*.h imgui/backends/imgui_impl_glfw.h imgui/backends/imgui_impl_opengl3.h)

file(GLOB PROJECT_SOURCES src/*.cpp)
add_library(imgui ${IMGUI_SRC} ${IMGUI_HEADERS} ${PROJECT_SOURCES})




add_subdirectory(libs/glfw)
#add_subdirectory(ImGuiFileDialog)

include_directories(
        imgui
        imgui/backends
        ${PROJECT_SOURCE_DIR}/libs/glfw/include
)

add_executable(smelyzajko-gui ImGuiFileDialog/ImGuiFileDialog.cpp  main.cpp src/ConfigurationMode.cpp src/widgets/Element.cpp
src/widgets/Slider.cpp src/widgets/Button.cpp src/widgets/Checkbox.cpp src/widgets/Label.cpp
src/widgets/MultiLineLabel.cpp src/widgets/Rectangle.cpp src/widgets/SingleLineLabel.cpp)


target_link_libraries(smelyzajko-gui PRIVATE imgui glfw opengl32  )
#=======
## Link the ImGui, OpenGL, and GLFW libraries
#target_link_libraries(smelyzajko-gui PRIVATE imgui glfw GL ImGuiFileDialog)

