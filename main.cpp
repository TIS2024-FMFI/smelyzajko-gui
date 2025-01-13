#include "src/ConfigurationMode.h"
#include "src/OperatingMode.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " (--config | --operate) [config_file.yaml]\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string configFile = (argc == 3) ? argv[2] : "";

    YAML::Node config = YAML::LoadFile(configFile);

    if (mode == "--config") {
        if (configFile.empty()) {
            std::cerr << "Error: Missing configuration file for --config mode.\n";
            return 1;
        }

        ConfigurationMode gui(config);
        gui.run();
    } else if (mode == "--operate") {
        if (configFile.empty()) {
            std::cerr << "Error: Missing configuration file for --operate mode.\n";
            return 1;
        }

        OperatingMode operatingMode;
        operatingMode.run();
    } else {
        std::cerr << "Invalid flag. Use --config or --operate.\n";
        return 1;
    }

    return 0;
}