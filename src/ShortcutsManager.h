// src/ShortcutsManager.h
#ifndef SHORTCUTS_MANAGER_H
#define SHORTCUTS_MANAGER_H

#include <unordered_map>
#include <functional>
#include <string>
#include <GLFW/glfw3.h>

class ShortcutsManager {
public:
    using ShortcutCallback = std::function<void()>;

    explicit ShortcutsManager(GLFWwindow* window) : window(window) {}

    ShortcutsManager();

    void registerShortcut(const std::string& shortcut, ShortcutCallback callback);
    void processShortcuts();
    void setWindow(GLFWwindow* window);

private:
    std::unordered_map<std::string, ShortcutCallback> shortcuts;
    GLFWwindow* window;

    bool isShortcutPressed(const std::string& shortcut);
};

#endif // SHORTCUTS_MANAGER_H