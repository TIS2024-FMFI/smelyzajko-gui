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
        libxi-dev \
        libwayland-dev
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
        libxkbcommon-devel \
        pkgconfig \
        libXinerama-devel \
        libXcursor-devel \
        libXi-devel \
        wayland-devel
}

# Function to install compilers for all distributions
install_compilers() {
    echo "Installing necessary compilers..."

    # Check for Ubuntu/Debian
    if [ -f /etc/os-release ] && { grep -q "ID=ubuntu" /etc/os-release || grep -q "ID=debian" /etc/os-release; }; then
        sudo apt-get install -y build-essential
    # Check for Fedora
    elif [ -f /etc/os-release ] && grep -q "ID=fedora" /etc/os-release; then
        sudo dnf install -y gcc-c++ gcc
    else
        echo "Unsupported distribution for compiler installation"
        exit 1
    fi
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
else
    echo "Unsupported Linux distribution: $DISTRO"
    exit 1
fi

install_compilers

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
echo "./smelyzajko-gui (--config | --operate | --replay) [path for config_file.yaml]"
