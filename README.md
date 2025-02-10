# smelyzajko-gui
The smelyzajko-gui is a graphical user interface designed to consolidate information from various robot modules into a single, user-friendly window. In modular robotic control systems, each module processes data from specific sensors or systems, or implements a part of the overall control algorithm. 

Traditionally, these modules display information in separate windows, leading to a cluttered interface. This GUI addresses the issue by presenting all module data within one screen, allowing users to adjust the size of each visual output and switch between them seamlessly. Additionally, the program offers a replay mode, enabling users to review and analyze previous sessions for improved diagnostics and performance evaluation.

This project utilizes Dear ImGui, a graphical user interface library for C++. Link to the library repository [here](https://github.com/ocornut/imgui).

This project is part of the Tvorba Informačných Systémov (TIS) course at the Faculty of Mathematics, Physics and Informatics, Comenius University in Bratislava.

# Installation Using Script for `smelyzajko-gui`
## Works for Ubuntu/Debian/Fedora-based linux distibutions 

## Step 1: Clone the Repository

1. Open a terminal window.
2. Navigate to the directory where you want to clone the project.
3. Run the following command to clone the repository:
   ```bash
   git clone https://github.com/TIS2024-FMFI/smelyzajko-gui.git
   ```
4. Navigate into the cloned repository:
    ```bash
   cd smelyzajko-gui
   ```

## Step 2: Make the Setup Script Executable

1. Make the setup script executable by running the following command:

    ```bash
    chmod +x setup_project.sh
    ```

## Step 3: Run the Setup Script

1. Now, you can run the script to install the dependencies and build the project:

    ```bash
    ./setup_project.sh
    ```

## Step 4: Running the Project

1. After the script completes, you can run the program.
2. Change to the `build` directory (if you are not already in it):
   ```bash
   cd build
   ```
3. Run the executable with one of the following options:
    - **To run the program in configuration mode**:
      ```bash
      ./smelyzajko-gui --config [path to config_file.yaml]
      ```
    - **To run the program in operation mode**:
      ```bash
      ./smelyzajko-gui --operate [path to config_file.yaml]
      ```
    - **To run the program in replay mode**:
      ```bash
      ./smelyzajko-gui --replay [path to config_file.yaml]
      ```


# Manual Installation Instructions for `smelyzajko-gui`

This guide provides step-by-step instructions on how to manually install the necessary dependencies and build the `smelyzajko-gui` project for different Linux distributions.

## Step 1: Clone the Repository

1. Open a terminal window.
2. Navigate to the directory where you want to clone the project.
3. Run the following command to clone the repository:
   ```bash
   git clone https://github.com/TIS2024-FMFI/smelyzajko-gui.git
   ```
4. Navigate into the cloned repository:
    ```bash
   cd smelyzajko-gui
   ```

## Step 2: Install Dependencies

### For Ubuntu/Debian-based Distributions

1. Update your package list:
   ```bash
   sudo apt-get update
   ```
2. Install the required dependencies:
   ```bash
   sudo apt-get install -y build-essential cmake git libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libxkbcommon-dev pkg-config libxinerama-dev libxcursor-dev libxi-dev
   ```

### For Fedora-based Distributions

1. Update your package list:
   ```bash
   sudo dnf update
   ```

2. Install the required dependencies:
   ```bash
   sudo dnf install -y cmake git glfw-devel mesa-libGL-devel mesa-libGLU-devel libxkbcommon-devel pkgconfig libXinerama-devel libXcursor-devel libXi-devel wayland-devel
   ```

## Step 3: Prepare the Build Directory

1. Open a terminal window in the root directory of the project.
2. Create a new directory named `build`:
   ```bash
   mkdir build
   ```
3. Change into the `build` directory:
   ```bash
   cd build
   ```

## Step 4: Configure the Project with CMake

1. Run CMake to configure the project:
   ```bash
   cmake ..
   ```

## Step 5: Build the Project

1. Once CMake has finished configuring the project, run `make` to compile the project:
   ```bash
   make
   ```

## Step 6: Running the Project

1. After building the project, you can run the program.
2. Change to the `build` directory (if you are not already in it):
   ```bash
   cd build
   ```
3. Run the executable with one of the following options:
   - **To run the program in configuration mode**:
     ```bash
     ./smelyzajko-gui --config [path to config_file.yaml]
     ```
   - **To run the program in operation mode**:
     ```bash
     ./smelyzajko-gui --operate [path to config_file.yaml]
     ```
   - **To run the program in replay mode**:
     ```bash
     ./smelyzajko-gui --replay [path to config_file.yaml]
     ```

