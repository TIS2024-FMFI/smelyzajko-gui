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
#file(GLOB PROJECT_SOURCES src/*/*.cpp src/*.cpp TestModules/*.cpp)
#
## Add ImGui as a library
#add_library(imgui ${IMGUI_SRC} ${IMGUI_HEADERS} ${PROJECT_SOURCES})
#
## Include directories
#include_directories(imgui imgui/backends ${CMAKE_SOURCE_DIR})
#
## Create the main executable
#add_executable(smelyzajko-gui ImGuiFileDialog/ImGuiFileDialog.cpp main.cpp)
#
## Link the ImGui, OpenGL, and GLFW libraries
#target_link_libraries(smelyzajko-gui PRIVATE imgui glfw GL)


cmake_minimum_required(VERSION 3.10)
project(smelyzajko-gui)



file(GLOB IMGUI_SRC libs/imgui/*.cpp libs/imgui/backends/imgui_impl_glfw.cpp libs/imgui/backends/imgui_impl_opengl3.cpp)
file(GLOB IMGUI_HEADERS libs/imgui/*.h libs/imgui/backends/imgui_impl_glfw.h libs/imgui/backends/imgui_impl_opengl3.h)
file(GLOB PROJECT_SOURCES src/*/*.cpp src/*.cpp TestModules/*.cpp)




add_library(imgui ${IMGUI_SRC} ${IMGUI_HEADERS} ${PROJECT_SOURCES})

include_directories(libs/imgui libs/imgui/backends ${CMAKE_SOURCE_DIR} libs/ImGuiFileDialog libs/yaml-cpp/include)


include_directories(
        libs/imgui
        libs/imgui/backends
        ${PROJECT_SOURCE_DIR}/libs/glfw/include
)

add_subdirectory(libs/glfw)
add_subdirectory(libs/yaml-cpp)

add_executable(smelyzajko-gui main.cpp libs/ImGuiFileDialog/ImGuiFileDialog.cpp src/ConfigurationMode.cpp src/widgets/Element.cpp
        src/widgets/Slider.cpp src/widgets/Button.cpp src/widgets/Checkbox.cpp src/widgets/Label.cpp
        src/widgets/MultiLineLabel.cpp src/widgets/Rectangle.cpp src/widgets/SingleLineLabel.cpp
        src/ShortcutsManager.cpp
)


target_link_libraries(smelyzajko-gui PRIVATE imgui glfw opengl32 yaml-cpp)
target_include_directories(smelyzajko-gui PRIVATE ${CMAKE_SOURCE_DIR}/libs/ImGuiFileDialog)




