#include "src/ConfigurationMode.h"
#include "src/OperatingMode.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " --config | --operate\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "--config") {
        ConfigurationMode gui;
        gui.run();
    } else if (mode == "--operate") {
        OperatingMode operatingMode;
        operatingMode.run();
    } else {
        std::cerr << "Invalid flag. Use --config or --operate.\n";
        return 1;
    }

    return 0;
}