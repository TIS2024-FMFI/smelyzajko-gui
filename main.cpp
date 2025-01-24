#include "src/ConfigurationMode.h"
#include "src/OperatingMode.h"
#include "src/ReplayMode.h"
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

std::string getFirstConfigFile(const std::string& directory) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().filename() == "config1.yaml") {
            return entry.path().string();
        }
    }
    return ""; // Ak sa nenašiel súbor config1.yaml
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " (--config | --operate | --replay) [path for config1.yaml]\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string configFile;

    // Ak nie je špecifikovaný súbor, nájde prvý config1.yaml
    if (argc == 3) {
        configFile = argv[2];
    } else {
        configFile = getFirstConfigFile("../config-files");
        if (configFile.empty()) {
            std::cerr << "Error: No configuration file provided and no `config1.yaml` file found in the `config-files` directory.\n";
            return 1;
        }
    }

    // Načítanie YAML konfigurácie
    YAML::Node config;
    try {
        config = YAML::LoadFile(configFile);
        config["configFile"] = configFile; // Uloží cestu ku konfigurácii
    } catch (const std::exception& e) {
        std::cerr << "Error loading configuration file: " << e.what() << "\n";
        return 1;
    }

    ImGui::CreateContext();
    ImGui::SetCurrentContext(ImGui::GetCurrentContext());
    config["mode"] = mode;
    // Spustenie podľa režimu
    if (mode == "--config") {
        ConfigurationMode gui(config);
        gui.run();
    } else if (mode == "--operate") {
        OperatingMode operatingMode(config);
        operatingMode.run();
    } else if (mode == "--replay") {
        ReplayMode replayMode(config);
        replayMode.run();
    } else {
        std::cerr << "Invalid flag. Use --config, --operate, or --replay.\n";
        return 1;
    }

    return 0;
}
