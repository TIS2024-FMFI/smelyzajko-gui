#!/bin/bash

# Exit on error
set -e

# Function to install dependencies for Ubuntu/Debian-based distributions
install_ubuntu_debian_dependencies() {
    echo "Installing dependencies for Ubuntu/Debian..."
    sudo apt-get update
    sudo apt-get install -y \
        build-essential \
        cmake \
        git \
        libglfw3-dev \
        libgl1-mesa-dev \
        libglu1-mesa-dev \
        libxkbcommon-dev \
        pkg-config \
        libxinerama-dev \
        libxcursor-dev \
        libxi-dev
}

# Function to install dependencies for Fedora-based distributions
install_fedora_dependencies() {
    echo "Installing dependencies for Fedora..."
    sudo dnf install -y \
        cmake \
        git \
        glfw-devel \
        mesa-libGL-devel \
        mesa-libGLU-devel \
        libXkbcommon-devel \
        pkgconfig \
        libXinerama-devel \
        libXcursor-devel \
        libXi-devel
}

# Function to install dependencies for Arch-based distributions
install_arch_dependencies() {
    echo "Installing dependencies for Arch Linux..."
    sudo pacman -Syu --noconfirm
    sudo pacman -S --noconfirm \
        cmake \
        git \
        glfw-wayland \
        mesa \
        libxkbcommon \
        pkgconf \
        libxinerama \
        libxcursor \
        libxi
}

# Detect the distribution
if [ -f /etc/os-release ]; then
    . /etc/os-release
    DISTRO=$ID
fi

# Install dependencies based on distribution
if [ "$DISTRO" == "ubuntu" ] || [ "$DISTRO" == "debian" ]; then
    install_ubuntu_debian_dependencies
elif [ "$DISTRO" == "fedora" ]; then
    install_fedora_dependencies
elif [ "$DISTRO" == "arch" ] || [ "$DISTRO" == "manjaro" ]; then
    install_arch_dependencies
else
    echo "Unsupported Linux distribution: $DISTRO"
    exit 1
fi

# Create the build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

# Change to the build directory
cd build

# Run CMake to configure the project
echo "Configuring the project with CMake..."
cmake ..

# Build the project
echo "Building the project..."
make

# Run the executable
echo "The project has been built."
echo "To open the program, do 'cd build' and:"
echo "./smelyzajko-gui (--config | --operate) [path for config_file.yaml]"
