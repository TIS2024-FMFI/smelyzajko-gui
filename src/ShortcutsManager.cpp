#include <iostream>
#include "ShortcutsManager.h"

void ShortcutsManager::registerShortcut(const std::string& shortcut, ShortcutCallback callback) {
    shortcuts[shortcut] = callback;
}

void ShortcutsManager::processShortcuts() {
    if (!window) {
        std::cerr << "Error: window is not initialized." << std::endl;
        return;
    }
    for (const auto& [shortcut, callback] : shortcuts) {
        if (isShortcutPressed(shortcut)) {
            callback();
        }
    }
}

bool ShortcutsManager::isShortcutPressed(const std::string& shortcut) {
    if (!window) {
        std::cerr << "Error: window is not initialized." << std::endl;
        return false;
    }

    if (shortcut == "Ctrl+G") {
        return (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) &&
               glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS;
    } else if (shortcut == "Ctrl+E") {
        return (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) &&
               glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
    }
    // Add more shortcuts as needed
    return false;
}

void ShortcutsManager::setWindow(GLFWwindow* window) {
    this->window = window;
}

ShortcutsManager::ShortcutsManager() {}
