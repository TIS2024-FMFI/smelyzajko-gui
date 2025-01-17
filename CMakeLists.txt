#cmake_minimum_required(VERSION 3.10)
#project(smelyzajko-gui)
#
#set(CMAKE_CXX_STANDARD 17)
#
## ImGui source files
#file(GLOB IMGUI_SRC libs/imgui/*.cpp libs/imgui/backends/imgui_impl_glfw.cpp libs/imgui/backends/imgui_impl_opengl3.cpp)
#file(GLOB IMGUI_HEADERS libs/imgui/*.h libs/imgui/backends/imgui_impl_glfw.h libs/imgui/backends/imgui_impl_opengl3.h)
#
## Automatically collect all source files in src/
#file(GLOB PROJECT_SOURCES src/*/*.cpp src/*.cpp TestModules/*.cpp)
#
## Add ImGui as a library
#add_library(imgui ${IMGUI_SRC} ${IMGUI_HEADERS} ${PROJECT_SOURCES})
#
## Include directories
#add_subdirectory(libs/yaml-cpp)
#include_directories(libs/imgui libs/imgui/backends ${CMAKE_SOURCE_DIR} libs/ImGuiFileDialog libs/yaml-cpp/include /opt/homebrew/include ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR})
#include_directories(
#        libs/imgui
#        libs/imgui/backends
#        ${PROJECT_SOURCE_DIR}/libs/glfw/include
#)
#
##find_package(glfw3 REQUIRED)
## Create the main executable
#add_executable(smelyzajko-gui libs/ImGuiFileDialog/ImGuiFileDialog.cpp main.cpp)
#
## Link the ImGui, OpenGL, GLFW, and yaml-cpp libraries
#if(APPLE)
#    add_definitions(-DGL_SILENCE_DEPRECATION)
#    find_library(OPENGL_FRAMEWORK OpenGL)
#    target_link_libraries(smelyzajko-gui PRIVATE imgui ${OPENGL_FRAMEWORK} glfw yaml-cpp)
#else()
#    target_link_libraries(smelyzajko-gui PRIVATE imgui glfw opengl32 yaml-cpp)
#endif()
#
#target_include_directories(smelyzajko-gui PRIVATE ${CMAKE_SOURCE_DIR}/libs/ImGuiFileDialog)
#
#
#############################################


cmake_minimum_required(VERSION 3.10)
project(smelyzajko-gui)

#set(CMAKE_CXX_STANDARD 17)


#FetchContent_Declare(
#        yaml-cpp
#        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
#        GIT_TAG        master # Use a specific tag or branch if needed
#)
#
#FetchContent_MakeAvailable(yaml-cpp)


file(GLOB IMGUI_SRC libs/imgui/*.cpp libs/imgui/backends/imgui_impl_glfw.cpp libs/imgui/backends/imgui_impl_opengl3.cpp)
file(GLOB IMGUI_HEADERS libs/imgui/*.h libs/imgui/backends/imgui_impl_glfw.h libs/imgui/backends/imgui_impl_opengl3.h)
file(GLOB PROJECT_SOURCES src/*/*.cpp src/*.cpp TestModules/*.cpp)




add_library(imgui ${IMGUI_SRC} ${IMGUI_HEADERS} ${PROJECT_SOURCES}
        src/ShortcutsManager.h
        src/ShortcutsManager.cpp)

include_directories(libs/imgui libs/imgui/backends ${CMAKE_SOURCE_DIR} libs/ImGuiFileDialog libs/yaml-cpp/include)


include_directories(
        libs/imgui
        libs/imgui/backends
        ${PROJECT_SOURCE_DIR}/libs/glfw/include
)

add_subdirectory(libs/glfw)
add_subdirectory(libs/yaml-cpp)

add_executable(smelyzajko-gui main.cpp libs/ImGuiFileDialog/ImGuiFileDialog.cpp
)


target_link_libraries(smelyzajko-gui PRIVATE imgui glfw opengl32 yaml-cpp)
target_include_directories(smelyzajko-gui PRIVATE ${CMAKE_SOURCE_DIR}/libs/ImGuiFileDialog)



