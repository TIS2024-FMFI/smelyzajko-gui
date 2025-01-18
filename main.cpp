#include "src/ConfigurationMode.h"
#include "src/OperatingMode.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

std::string getFirstConfigFile(const std::string& directory) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".yaml") {
            return entry.path().string();
        }
    }
    return ""; // Return an empty string if no valid file is found
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " (--config | --operate) [path for config_file.yaml]\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string configFile;

    // If no config file is provided, choose the first one in the "config-files" directory
    if (argc == 3) {
        configFile = argv[2];
    } else {
        configFile = getFirstConfigFile("../config-files");
        if (configFile.empty()) {
            std::cerr << "Error: No configuration file provided and no `.yaml` files found in the `config-files` directory.\n";
            return 1;
        }
    }

    // Load the YAML configuration
    YAML::Node config;
    try {
        config = YAML::LoadFile(configFile);
        config["configFile"] = configFile;
    } catch (const std::exception& e) {
        std::cerr << "Error loading configuration file: " << e.what() << "\n";
        return 1;
    }

    ImGui::CreateContext();  // Create the ImGui context here
    ImGui::SetCurrentContext(ImGui::GetCurrentContext());  // Ensure that we are setting the context if needed

    // Determine the mode
    if (mode == "--config") {
        ConfigurationMode gui(config);
        gui.run();
    } else if (mode == "--operate") {
        OperatingMode operatingMode(config);
        operatingMode.run();
    } else {
        std::cerr << "Invalid flag. Use --config or --operate.\n";
        return 1;
    }

    return 0;
}