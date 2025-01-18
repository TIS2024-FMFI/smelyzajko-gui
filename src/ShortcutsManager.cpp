#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include "ShortcutsManager.h"

void ShortcutsManager::registerShortcut(const std::string& shortcut, ShortcutCallback callback) {
    shortcuts[shortcut] = callback;
}

void ShortcutsManager::processShortcuts() {
    if (!window) {
        std::cerr << "Error: window is not initialized." << std::endl;
        return;
    }
    if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
        // Process shortcuts only when window is focused
        for (const auto& [shortcut, callback] : shortcuts) {
            if (isShortcutPressed(shortcut)) {
                callback();
            }
        }
    }

}

bool ShortcutsManager::isShortcutPressed(const std::string& shortcut) {
    if (!window) {
        std::cerr << "Error: window is not initialized." << std::endl;
        return false;
    }

    std::vector<std::string> keys = parseShortcut(shortcut);

    for (const std::string& key : keys) {
        int glfwKey = getGlfwKey(key);
        if (glfwKey == -1 || glfwGetKey(window, glfwKey) != GLFW_PRESS) {
            return false;
        }
    }

    // Debounce mechanism
    auto now = std::chrono::steady_clock::now();
    if (lastActivationTimes.find(shortcut) != lastActivationTimes.end()) {
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                now - lastActivationTimes[shortcut]).count();

        if (elapsedTime < DEBOUNCE_MS) {
            return false; // Too soon since the last activation
        }
    }

    lastActivationTimes[shortcut] = now; // Update the last activation time
    return true;
}

std::vector<std::string> ShortcutsManager::parseShortcut(const std::string& shortcut) {
    std::vector<std::string> keys;
    std::istringstream ss(shortcut);
    std::string key;

    // Split the shortcut string by '+' delimiter
    while (std::getline(ss, key, '+')) {
        keys.push_back(key);
    }

    return keys;
}

int ShortcutsManager::getGlfwKey(const std::string& key) {
    // Map string keys to GLFW key codes
    if (key == "Ctrl") return GLFW_KEY_LEFT_CONTROL;
    if (key == "Cmd") return GLFW_KEY_LEFT_SUPER;
    if (key == "CmdR") return GLFW_KEY_RIGHT_SUPER;

    if (key == "A") return GLFW_KEY_A;
    if (key == "S") return GLFW_KEY_S;
    if (key == "D") return GLFW_KEY_D;
    if (key == "F") return GLFW_KEY_F;
    if (key == "G") return GLFW_KEY_G;
    if (key == "Q") return GLFW_KEY_Q;
    if (key == "W") return GLFW_KEY_W;
    if (key == "E") return GLFW_KEY_E;
    if (key == "R") return GLFW_KEY_R;

    if (key == "1") return GLFW_KEY_1;
    if (key == "2") return GLFW_KEY_2;
    if (key == "3") return GLFW_KEY_3;
    if (key == "4") return GLFW_KEY_4;
    if (key == "5") return GLFW_KEY_5;
    if (key == "6") return GLFW_KEY_6;
    if (key == "7") return GLFW_KEY_7;
    if (key == "8") return GLFW_KEY_8;
    if (key == "9") return GLFW_KEY_9;
    if (key == "0") return GLFW_KEY_0;

    if (key == "Left") return GLFW_KEY_LEFT;
    if (key == "Right") return GLFW_KEY_RIGHT;

    return -1; // Unknown key
}

void ShortcutsManager::setWindow(GLFWwindow* window) {
    this->window = window;
}

ShortcutsManager::ShortcutsManager() {}
