cmake_minimum_required(VERSION 3.5)

project(ImGuiFileDialog)

add_library(ImGuiFileDialog STATIC
        ImGuiFileDialog.cpp
        ImGuiFileDialog.h
        ImGuiFileDialogConfig.h
)

target_include_directories(ImGuiFileDialog PUBLIC .)

if(UNIX)
    target_compile_options(ImGuiFileDialog PUBLIC -Wno-unknown-pragmas)
endif()
