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
    if (key == "Ctrl") return GLFW_KEY_LEFT_CONTROL;
    if (key == "Cmd") return GLFW_KEY_LEFT_SUPER;
    if (key == "CmdR") return GLFW_KEY_RIGHT_SUPER;
    if (key == "Space") return GLFW_KEY_SPACE;
    if (key == "Enter") return GLFW_KEY_ENTER;
    if (key == "Escape") return GLFW_KEY_ESCAPE;
    if (key == "Backspace") return GLFW_KEY_BACKSPACE;

    if (key.size() == 1 && std::isalpha(key[0])) {
        return GLFW_KEY_A + (std::toupper(key[0]) - 'A');
    }
    if (key.size() == 1 && std::isdigit(key[0])) {
        return GLFW_KEY_0 + (key[0] - '0');
    }

    if (key == "=") return GLFW_KEY_EQUAL;
    if (key == "-") return GLFW_KEY_MINUS;

    if (key == "Left") return GLFW_KEY_LEFT;
    if (key == "Right") return GLFW_KEY_RIGHT;
    if (key == "Up") return GLFW_KEY_UP;
    if (key == "Down") return GLFW_KEY_DOWN;

    // If no match, return -1
    return -1;
}

void ShortcutsManager::setWindow(GLFWwindow* window) {
    this->window = window;
}

ShortcutsManager::ShortcutsManager() {}
