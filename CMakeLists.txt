cmake_minimum_required(VERSION 3.10)
project(smelyzajko-gui)

set(CMAKE_CXX_STANDARD 17)

file(GLOB IMGUI_SRC
        libs/imgui/*.cpp
        libs/imgui/backends/imgui_impl_glfw.cpp
        libs/imgui/backends/imgui_impl_opengl3.cpp
)

file(GLOB IMGUI_HEADERS
        libs/imgui/*.h
        libs/imgui/backends/imgui_impl_glfw.h
        libs/imgui/backends/imgui_impl_opengl3.h
)


file(GLOB PROJECT_SOURCES
        src/*/*.cpp
        src/*.cpp
        TestModules/*.cpp
)

add_library(imgui STATIC ${IMGUI_SRC} ${IMGUI_HEADERS} ${PROJECT_SOURCES})

include_directories(
        libs/imgui
        libs/imgui/backends
        ${CMAKE_SOURCE_DIR}
        libs/ImGuiFileDialog
        libs/yaml-cpp/include
        /opt/homebrew/include
        ${PROJECT_SOURCE_DIR}/src
)

add_subdirectory(libs/yaml-cpp)

add_executable(smelyzajko-gui
        libs/ImGuiFileDialog/ImGuiFileDialog.cpp
        main.cpp
)

if(APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION)
    find_library(OPENGL_FRAMEWORK OpenGL)
    target_link_libraries(smelyzajko-gui PRIVATE imgui ${OPENGL_FRAMEWORK} glfw yaml-cpp)
else()
    target_link_libraries(smelyzajko-gui PRIVATE imgui glfw GL yaml-cpp)
endif()

target_include_directories(smelyzajko-gui PRIVATE ${CMAKE_SOURCE_DIR}/libs/ImGuiFileDialog)